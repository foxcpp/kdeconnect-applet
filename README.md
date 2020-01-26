# kdeconnect-applet

Minimialistic DE-independent GUI frontend for KDE Connect.

kdeconnect-indicator does not expose all KDE Connect features.  KDE Connect
package includes the "plasmoid" implementation that provides remaining bits,
but they all require running Plasma DE which is not what some people want.

## Features

- Terrible non-idiomatic Qt-based code.
- Remote Keyboard implementation.
- Request photo from device camera.

## Installation

```
mkdir build; cd build
cmake ..
make -j4
sudo make install
```

## Dependencies

- Qt 5 Core, GUI, Widgets and D-Bus modules.
- CMake 3.4.
- Reasonably up-to-date C++ compiler.

I dunno what minimal versions are, but it works well with Qt 5.14, CMake 3.16
and GCC 9.2.0.

## License

MIT
