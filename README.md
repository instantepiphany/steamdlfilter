#Steamdlfilter

Steamdlfilter - ensure steam only downloads from your ISP's unmetered content server.

Steamdlfilter is a collection of 3 things, a library (steamdlfilter.so, see compiling 2.), a graphical configuration utility to disable and enable the filter easily (and more soon), and a simple launcher script that launches steam with the filter attached.

The library wraps the send(2) function, which among other things, steam uses to request chunks of games to download from various steam servers. It simply checks if the chunk is being requested from your chosen server, and only allows the connection if so.


The configuration utility is a tiny gtk3 program that so far only has a toggle button so you can easily disable/enable the filter. You can enable and disable the filter while you have steam running, and while downloading steam games, however it will only take effect for new "chunks" of data that are requested. In my testing though, downloading Prison Architect and Dota 2, the chunks tended to be small and so within 20 seconds or so, the desired filter state was achieved.  In future the configuration utility will have a list of servers that you can modify, that steam will be restricted to when the filter is enabled(currently you can only have 1 unmetered server configured, and you must edit the config file to change it).

###Installation

####Dependencies

64-bit systems will need to have both 32 and 64-bit libconfig and libconfig-dev/devel installed.

gtk3, glibc-devel/linux-glibc-devel are also required.

Note, the only things you *need* to compile and install are the library (steamdlfilter.so, see the second part of compiling) and its dependencies.

The config utility is optional, it simply provides an easy way to toggle the filter on or off (which works while steam is running and takes effect quickly in every test so far).

####Compiling

1. steamdlfiltercfg: ``gcc -lconfig `pkg-config --cflags gtk+-3.0` -o steamdlfiltercfg steamdlfiltercfg.c `pkg-config --libs gtk+-3.0` -Wall``
2. steamdlfilter: ``gcc -Wall -m32 -shared -fPIC -L/usr/local/lib -lconfig -ldl steamdlfilter.c -o steamdlfilter.so -D_GNU_SOURCE``

####Misc

1.  Copy `config` to `$HOME/.config/steamdlfilter/` and modify the unmetered server value and the `logging` value to your liking
2. To launch: `DEBUGGER="env LD_PRELOAD=/path/to/steamdlfilter/steamdlfilter.so" LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib steam` or alternatively use the included script. steamdlfilter.sh which will look for the library in the current directory

Note: In my testing, I succesfully downloaded an update for Prison Architect, and all of Dota 2, and had 0 data leakage, however I make NO guarantees whatsoever that this will work properly for you. Your mileage may vary.
