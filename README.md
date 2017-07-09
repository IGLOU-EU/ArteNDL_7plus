# ![Arte logo](https://static-cdn.arte.tv/guide/favicons/favicon-96x96.png) **ArteNatifDownLoader (ArteNDL)**

> Arte Natif DownLoader is writed on C
Current version is **1.1** (07/2017)

:page_with_curl: **DESCRIPTION** 
---
ArteNDL is a simple and native video downloader tools for Arte replay website.
It's working properly for other Arte video content, but i didn't try everything.

You can also use it like streamer view with mpv, vlc, or whatever.

:rocket: **HOW TO** 
---
Go to http://www.arte.tv and choose you'r video.
On video url *(www.arte.tv/.../051058-000-A/...)*, take the video ID **051058-000-A**.

For view quality, language and infos: 

    $ ./ArteNDL -a 051058-000-A (-l <language|fr(default)/de/en>)

Result:
>Title: Room 237
Vid Time (mn): 99
Time up: 10/07/2017 03:00:00 +0000
Description: Sorti en 1980, "Shining", de Stanley Kubrick est devenu rapidement un classique du cinéma d'épouvante. Il a toujours été l'objet de théories plus ou moins farfelues sur de réels ou prétendus messages cachés qu'il véhiculerait. Ce documentaire croustillant les explore...

>HTTPS_MP4_MQ_2: Allemand (Doublé) 
HTTPS_MP4_EQ_2: Allemand (Doublé) 
HTTPS_MP4_HQ_2: Allemand (Doublé) 
HTTPS_MP4_SQ_2: Allemand (Doublé) 
HTTPS_MP4_MQ_1: Français (Doublé) 
HTTPS_MP4_HQ_1: Français (Doublé) 
HTTPS_MP4_EQ_1: Français (Doublé) 
HTTPS_MP4_SQ_1: Français (Doublé) 

>Milia hominum frui culturae, gratias ARTE.


Chose quality:
>High: ____SQ
Standard: EQ
Low: ____HQ

Download or stream and let's go:
*ArteNDL -a [video id] -q [quality/language id] -s [play on]*

    $ ./ArteNDL -a 053986-001-A -q HTTP_MP4_MQ_1 -s mpv

Print help:

    $ ./ArteNDL -h;

:construction: **INSTALLATION** 
---
Lib required: **json-c**, **curl**;

Clone repository, or download ArteNDL.c

Compilation: **$ gcc ArteNDL.c -lcurl -ljson-c -o ArteNDL**
Tested on: **Gnu/Linux** [32/64bit] and **OpenBSD** 5.1+

:question: **FAQ** 
---
**When I should use this program ?**

- Bad Internet connections
- No Internet connections (Travelling ?)
- Poor power or Old computer

You can download videos to later, or stream in a lite video player =D

**Why Arte ?**

I like Arte this is my favorite channel.
But, my computer is too old (2001) to look Arte.tv,
And I don't have a TV !

:shipit: **DISCLAIMER** 
-------
This program is for personal use only. You alone have the responsability for any usage which could be against ARTE privacy policy.

>![enter image description here](https://static-cdn.arte.tv/static/styleguide/2.7.1/src/components/footer/img/logo_arte.png)

>*Tous droits de reproduction et de diffusion réservés © 2017 ARTE G.E.I.E.*
*ARTE G.E.I.E. 4, quai du chanoine Winterer CS 20035 F- 67080 Strasbourg Cedex*

----------
