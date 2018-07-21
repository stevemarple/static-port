Static port
===========

A Linux library to force `bind()` system calls to use a pre-defined
port. Written primarily to fix the Linux Java version of Minecraft so
that "Open to LAN" uses a known port.


Usage
-----

Download a ZIP version from Github or clone the respository. In
Minecraft modify your profile to call `static-port.sh` instead of
Java. Edit `static-port.sh` if you wish to force Minecraft to use a
different port to 25565 when opening a game to LAN.


License
-------

Static port is licensed under the GNU Lesser General Public
License. See LICENSE.txt for details.

