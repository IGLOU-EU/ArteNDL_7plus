# ArteNatifDownLoader (ArteNDL)
Arte 7+ Natif DownLoader writed in C

Current version: **1.1**.

DESCRIPTION
---

Go to http://www.arte.tv, choose you'r video (ex: www.arte.tv/guide/fr/053986-001-A/exemple).

Take it the video ID "053986-001-A"

View quality, language and infos: $ ./ArteNDL -a 053986-001-A (-l <language>, fr(default)/de ("en" may be, it depends of arte.tv))

Chose quality, download or stream and let's go:
$ ./ArteNDL -a 053986-001-A -q HTTP_MP4_MQ_1 -s smplayer

Help: $ ./ArteNDL -h;

INSTALLATION
---

lib required: json-c, curl;

Compilation : $ gcc ArteNDL.c -lcurl -ljson-c -o ArteNDL
(Not tested on Windows and OsX)

FAQ
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

DISCLAIMER
-------

This program is for personal use only. You alone have the responsability for any usage which could be against ARTE privacy policy.
