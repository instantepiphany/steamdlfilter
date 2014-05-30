#Steamdlfilter

Steamdlfilter is a collection of a library, config utility and bash script that limits steam to downloading games only from your ISP's "unmetered" server/s.
It is similar in functionality (but more simple and with less polish, for now) to steamlimiter, which does much the same thing on windows.

Steamdlfilter is a simple shared library, and a configuration utility, and a script to launch steam (the linux port of steam, *not* steam for windows via WINE), with this library preloaded. The purpose of steamdlfilter is to restrict steam to downloading from "unmetered" content servers, if your ISP has them.  


The library wraps the send(2) function, which among other things, steam uses to request chunks of games to download from various steam servers. It simply checks if the chunk is being requested from your chosen server, and only allows the connection if so.


The configuration utility is a tiny gtk3 program that so far only has a toggle button so you can easily disable/enable the filter. You can enable and disable the filter while you have steam running, and while downloading steam games, however it will only take effect for new "chunks" of data that are requested. In my testing though, downloading Prison Architect and Dota 2, the chunks tended to be small and so within 20 seconds or so, the desired filter state was achieved.  In future the configuration utility will have a list of servers that you can modify, that steam will be restricted to when the filter is enabled(currently you can only have 1 unmetered server configured, and you must edit the config file to change it).

###Installation

####Dependencies

32-bit libconfig and gtk3 should be all you need.

####Compiling

1. steamdlfiltercfg: ``gcc -lconfig `pkg-config --cflags gtk+-3.0` -o steamdlfiltercfg steamdlfiltercfg.c `pkg-config --libs gtk+-3.0` -Wall``
2. steamdlfilter: ``gcc -Wall -m32 -shared -fPIC -L/usr/local/lib -lconfig -ldl steamdlfilter.c -o steamdlfilter.so -D_GNU_SOURCE``

####Misc

1.  Copy `config` to `$HOME/.config/steamdlfilter/` and modify the unmetered server value and the `logging` value to your liking
2. To launch: `DEBUGGER="env LD_PRELOAD=/path/to/steamdlfilter/steamdlfilter.so" LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib steam`

Note: In my testing, I succesfully downloaded an update for Prison Architect, and all of Dota 2, and had 0 data leakage, however I make NO guarantees whatsoever that this will work properly for you. Your mileage may vary.
