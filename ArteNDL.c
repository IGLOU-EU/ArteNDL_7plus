/**
 * Copyright 2016 Kara adrien
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * gcc ArteNDL.c -lcurl -ljson-c -o ArteNDL
 **/

#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <curl/curl.h>
#include <json-c/json.h>

#define JSON_GET_SVALUE (char *)json_object_get_string(val)

#define ANDL_NULL   0x00000000
#define HELP        0x00000001
#define INFOS       0x00000002
#define VERBOSE     0x00000004
#define DEBUG       0x00000008
#define STREAM      0x00000010
#define DOWNLOAD    0x00000020

#define str_equal(a, b) (strcmp(a, b) == 0 ? 1 : 0)
#define strn_equal(a, b, n) (strncmp(a, b, n) == 0 ? 1 : 0)
#define dup_test(var, warn) (var == NULL ? err_ext(warn) : 0)
#define err_ext(str) ({            \
                      perror(str); \
                      exit(EXIT_FAILURE);     \
                      })

/* ---- Global ---- */
char *df_jsonUrl  = "https://api.arte.tv/api/player/v1/config/{LG}/{ID}?platform=ARTEPLUS7";

char *language, *arteId, *vidId, *outPut, *videoUrl, *videoTitle;
uint32_t flags;

struct MemoryStruct {
    char *memory;
    size_t size;
} json;

/* ---- Proto ---- */
void   optParser  (int, const char *argv[]);
char  *str_replace(char *, char *, char *);
char  *uopen      (char *, int);
size_t curlWrBf   (void *, size_t, size_t, void *);
size_t curlWrFile (void *, size_t, size_t, void *);
void   jsonParser (char *, char *);
void   printInfos (char *, void *);
char  *argCpy     (char *, char *);

/* ---- Main ---- */
int main(int argc, const char *argv[])
{
    char *jsonUrl, *jsonData;

    /* argument parsing */
    flags = ANDL_NULL;
    optParser(argc, argv);

    /* Exception handling */
    if(arteId == NULL) {
        goto help;
    } else if(vidId == NULL && 0 == flags) {
        flags += INFOS;
    }

    if(vidId == NULL) {
        vidId = "false";
    } else if(0 == flags) {
        puts("Please, choose an action;\nInformations, Download or Stream.\n");
        goto help;
    }

    if(language == NULL) 
        language = "fr";

    /* Construct json url */
    jsonUrl = NULL;
    jsonUrl = (char *)calloc(strlen(df_jsonUrl) + strlen(language) + strlen(arteId) + 1, sizeof(char));
    strcpy(jsonUrl, df_jsonUrl);
    strcpy(jsonUrl, str_replace(jsonUrl, "{LG}", language));
    strcpy(jsonUrl, str_replace(jsonUrl, "{ID}", arteId));

    /* Get json file into jsonData var */
    jsonData = uopen(jsonUrl, 0);
    free(jsonUrl);

    /* Parse json */
    jsonParser(jsonData, "root");

    if(flags & DEBUG) {
        puts(videoTitle);
        puts(jsonData);
    }

    /* Selected outPut, stream or download*/
    if(NULL != videoUrl && NULL != outPut) {
        if(flags & STREAM) {
            system(argCpy(outPut, videoUrl));
        } else if(flags & DOWNLOAD) {
            uopen(videoUrl, 1);

            if(flags & VERBOSE)
                puts(argCpy("Video file outPut: ", argCpy(outPut, videoUrl)));
        }
    } else if(NULL == videoUrl && NULL != outPut) {
        puts("Error: Video url not find, probably bad quality ID.");
        goto exitError;
    }

    if(flags & HELP) {
        help:
        puts("Usage:    ArteNDL [options]\n\nOptions: (complete list)");
        puts(" -a <id>           Video ID on arte 7+ URL\n -q <quality_id>   Video quality (use -i for view all quality)\n -l <language>     Selecte json language (fr and de)\n -o <folder>       Output directory\n -s <player>       Output stream\n -i                Video informations\n -v                Verbose option (-vv for Debug)\n -h                Print this help\n");
        puts("Examples:");
        puts("ArteNDL -a 000000-000-A -i                                   Print video informations quality/language");
        puts("ArteNDL -a 000000-000-A -q HTTPS_MP4_MQ_1 -s \"mplayer -fs\"    Open video on mplayer in fullscreen");
        puts("ArteNDL -a 000000-000-A -q HTTPS_MP4_MQ_1 -o /tmp/arteout/    Download video inside arteout folder\n");
        goto exitError;
    }

    puts("\nMilia hominum frui culturae, gratias ARTE.");
    return EXIT_SUCCESS;

    exitError:
    return EXIT_FAILURE;
}

/* ---- Functions ---- */
void optParser (int argc, const char *argv[])
{
    int i = 1;

    if(argv[i] != NULL) {
        do {
            switch(argv[i][1]) {
                case 'i':
                    flags += INFOS;
                    break;
                case 'o':
                    ++i;
                    if('/' != argv[i][strlen(argv[i]) - 1]) {
                        outPut = argCpy((char *)argv[i], "/");
                    } else {
                        outPut = strdup(argv[i]);
                    }
                    flags     += DOWNLOAD;
                    break;
                case 'a':
                    ++i;
                    arteId     = strdup(argv[i]);
                    break;
                case 'q':
                    ++i;
                    vidId      = strdup(argv[i]);
                    break;
                case 'l':
                    ++i;
                    language   = strdup(argv[i]);
                    break;
                case 's':
                    ++i;
                    outPut     = strdup(argCpy((char *)argv[i], " "));
                    flags     += STREAM;
                    break;
                case 'v':
                    flags     += VERBOSE;

                    if('v' == argv[i][2])
                        flags     += DEBUG;
                case 'h':
                default:
                    flags     += HELP;
                    break;
            }
            ++i;
        } while(i < argc);
    }
}

char *str_replace(char *string, char *search, char *replace)
{
    char *tmp, *find;

    find = strstr(string, search);
    if(NULL == find)
        return strdup(string);

    tmp = NULL;
    tmp = (char *)calloc(strlen(replace) + strlen(string) + 1, sizeof(char));

    strcpy(tmp, strndup(string, find - string));
    strcat(tmp, replace);
    strcat(tmp, strlen(search) + find);
    strcpy(string, tmp);
    free(tmp);

    return string;
}

char *uopen(char *url, int download)
{
    CURL *curl;
    CURLcode curlCode;
    FILE *file;
    char *result;

    curl_global_init(CURL_GLOBAL_ALL);

    curl        = curl_easy_init();
    json.memory = malloc(1);
    json.size   = 0;

    if(curl)  {
        curl_easy_setopt(curl, CURLOPT_URL, url);

        if(flags & VERBOSE) {
            curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);            
        } else {
            curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
        }

        if(flags & DEBUG) {
            curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        } else {
            curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
        }

        if(download) {
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlWrFile);

            if(NULL == fopen(outPut, "r")) {
                puts(argCpy("Unable to write in: ", outPut));
                exit(EXIT_FAILURE);
            }

            file = fopen(argCpy(outPut, videoTitle), "wb");
            if(NULL != file) {
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
            } else {
                puts(argCpy("Unable to write in: ", outPut));
                exit(EXIT_FAILURE);
            }

        } else {
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlWrBf);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&json);
        }
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
        
        curlCode = curl_easy_perform(curl);
        
        if(CURLE_OK != curlCode) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(curlCode));
            exit(EXIT_FAILURE);
        }

        if(download)
            fclose(file);

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();

    result = strdup(json.memory);
    dup_test(result, "uopen(), result is NULL"); 
    free(json.memory);

    return result;
}

size_t curlWrBf(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;

    struct MemoryStruct *mem = (struct MemoryStruct *)userp;
 
    mem->memory = realloc(mem->memory, mem->size + realsize + 1);
    if(mem->memory == NULL) {
        /* out of memory! */ 
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }
 
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;
    
    return realsize;
}

size_t curlWrFile(void *ptr, size_t size, size_t nmemb, void *stream)
{
    size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
    return written;
}

void jsonParser(char *data, char *mother)
{
    struct json_object *jobj;
	int val_type;
    
    jobj = json_tokener_parse(data);

    json_object_object_foreach(jobj, key, val) {
		val_type = json_object_get_type(val);

		switch (val_type) {
			case json_type_int:
                printInfos(key, (int*)json_object_get_int(val));
				break;
			case json_type_string:
                if(strn_equal("HTTPS_", mother, (size_t)5) && str_equal("versionLibelle", key)) {
                    printInfos("Dispo", argCpy(JSON_GET_SVALUE, " \n"));
                } else if(str_equal("VRU", key)) {
                    printInfos("lastChance", JSON_GET_SVALUE);
                } else if(str_equal("VTI", key)) {
                    videoTitle = strdup(JSON_GET_SVALUE);
                    dup_test(videoTitle, "jsonParser(), videoTitle is NULL");
                } else if(str_equal(mother, vidId) && str_equal("url", key)) {
                    videoUrl   = strdup(JSON_GET_SVALUE);
                    dup_test(videoUrl, "jsonParser(), videoUrl is NULL");
                } else if(str_equal(mother, "custom_msg") && str_equal("msg", key)) {
                    printf("404: %s", JSON_GET_SVALUE);
                    exit(EXIT_FAILURE);
                }

                printInfos(key, JSON_GET_SVALUE);
				break;
			case json_type_object:
                if(strn_equal("HTTPS_", key, (size_t)5))
                    printInfos("Dispo", argCpy(key, ": "));

				jsonParser(JSON_GET_SVALUE, key);
				break;
		}
    }
}

void printInfos(char *key, void *str)
{
    if(flags & INFOS) {
        if(str_equal("VTI", key)) {
            printf("Title: %s\n", (char *)str);
        } else if(str_equal("V7T", key)) {
            printf("Description: %s\n\n", (char *)str);
        } else if(str_equal("VDU", key)) {
            printf("Vid Time (mn): %d\n", (int)str);
        } else if(str_equal("lastChance", key)) {
            printf("Time up: %s\n", (char *)str);
        }else if(str_equal("Dispo", key)) {
            printf("%s", (char *)str);
        } 
    }
}

char *argCpy(char *a, char *b)
{
    char *swap, *result;
    
    swap   = NULL;
    swap   = (char *)calloc(strlen(a) + strlen(b) + 1, sizeof(char));
    strcpy(swap, a);
    strcat(swap, b);

    result = NULL;
    result = strdup(swap);
    dup_test(result, "argCpy, result is NULL");

    free(swap);
    
    return result;
}
