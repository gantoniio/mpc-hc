# Media Player Classic - Home Cinema
[![GitHub Tag](https://img.shields.io/github/v/release/clsid2/mpc-hc?label=Latest+release)](https://github.com/clsid2/mpc-hc/releases/latest)
[![GitHub Tag](https://img.shields.io/github/v/release/clsid2/mpc-hc?include_prereleases&label=Pre-release)](https://github.com/clsid2/mpc-hc/releases)

**Media Player Classic - Home Cinema** (MPC-HC) is a free and open-source video and audio player for Windows. MPC-HC is based on the original Guliverkli project and contains many additional features and bug fixes.

## Main Features

MPC-HC is an extremely light-weight all around media player that supports all common video and audio file formats available for playback. 

*  Supports [EVR/EVR CP](https://en.wikipedia.org/wiki/Media_Foundation#Enhanced_Video_Renderer) (Enhanced Video Renderer),
*  Supports [HDR](https://en.wikipedia.org/wiki/High-dynamic-range_video) video (requires using [madvr](http://forum.doom9.org/showthread.php?t=146228) or [MPC Video Renderer](https://github.com/Aleksoid1978/VideoRenderer/releases)),
* Supports streaming directly from [Youtube](https://youtube.com/) and many other video websites,
* Supports most types of [subtitles](https://en.wikipedia.org/wiki/Subtitle_%28captioning%29#Subtitle_formats) including [BluRay](https://en.wikipedia.org/wiki/Blu-ray_Disc) .sup files, online subtitles search and download (hotkey `D`),
* Supports multiple monitors and autochange fullscreen modes,
* [On Screen Display (OSD)](https://en.wikipedia.org/wiki/On-screen_display) information,
* Playback and recording of television programs, if a supported [TV tuner card](https://en.wikipedia.org/wiki/TV_tuner_card) is installed,
* Video preview on the seekbar, quick seek (`Ctrl` + mouse scrollwheel), adjustable playback speed (0.25 - 8×),
* Video rotate, flip, mirror, stretch, zoom and predefined widescreen size profiles,
* A-B playback repeat  (press `[` and `]` to set start and stop markers)
* More than 20 [pixel shaders](https://en.wikipedia.org/wiki/Shader#Pixel_shaders) for video rendering special effects, [Color management](https://en.wikipedia.org/wiki/Color_management), VSync, option to remove [tearing](https://en.wikipedia.org/wiki/Screen_tearing),
* Built-in audio switcher with customizable channel mapping (up to 18 input channels),
* Web interface and MPC API for remote control, rich command line,
* Dark visual theme, customizable logo image,
* 42 translations available,
* _and more..._

## System Requirements

* An SSE2-capable CPU
* Windows Vista, 7, 8, 8.1, 10
* Both 32-bit and 64-bit systems are supported

## Development Status

The player is mature, stable, reliable, and already provides a huge amount of features.

Due to a lack of active developers, the player is currently in maintenance mode. This means that there are no direct plans for adding any big new features. Development is currently limited to small bug fixes and updates. However, external developers can still contribute additional fixes and new functionality. In fact, several people have already helped to give the player some new functionality and other improvements.

The internal codecs are developed in an external project ([LAV Filters](https://github.com/Nevcairiel/LAVFilters)). That project is still actively maintained, and MPC-HC will be updated periodically with the latest codecs. Same applies to code/files from other external projects that are used by MPC-HC, such as MediaInfo.

If you are a programmer and want to help improve MPC-HC, then you can do so by submitting a pull request containing your patches - for more detailed description please see our [contribution guide](CONTRIBUTING.md).

## License

MPC-HC's code is licensed under [GPL v3](/COPYING.txt).
