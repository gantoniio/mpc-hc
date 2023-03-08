*In compliance with the [GPL-3.0](https://opensource.org/licenses/GPL-3.0) license: I declare that this version of the program contains my modifications, which can be seen through the usual "git" mechanism.*  


2022-08  
Contributor(s):  
adipose  
clsid2  
>Re-apply volume after audio stream switch or graph clock change  
>improve efficiency of metrics lookups and font creation by caching (#1812)  
>Set lfCharSet value when creating font to use default character set  
>Apply sub delay to screenshots  
>.  
>fix parsing of background colors for webvtt (#1806)  
>Use buffer input instead of direct file open with MediaInfo in case or .rar  
- - - - - - - - - - - - - - - - - - - - - - - - - - - 


2022-07  
Contributor(s):  
Speeder  
adipose  
clsid2  
>Close current media sooner before doing YDL processing of next media  
>Let MediaInfo read file directly by default instead of giving it data though buffer  
>Fix issue with track name containing "off"  
>support unicode tooltips from opensubs (#1768)  
>Improve MPCPL saving for relative paths. Also works with subdirs now.  
>Fix another sub texture size issue  
>.  
>Ignore small drag distance when dragging video in windowed mode  
>support dark thumbnails for people using dark theme (#1782)  
>When saving a subtitle, the path is remembered (#1751)  
>Also do YDL processing when URL is opened through API  
- - - - - - - - - - - - - - - - - - - - - - - - - - - 


2022-06  
Contributor(s):  
Speeder  
adipose  
clsid2  
>Fix for commit 46d724e  
>Fix issue with FullscreenSeparateControls  where video window was destroyed after moving to next file  
>Show score column in sub download dialog  
>Command line /play without further parameters will now play last opened file.  
>Fix for commit 04105f7  
>Some missing code for commit 7df40ff  
>When changing video renderer, adjust the subtitle renderer to ISR when current selected  subtitle renderer is unavailable or is the inferior VSFilter.  
>Fix a crash  
>fix for evr-cp rotation being cut off (#1740)  
>Fix stupid copy paste typos in commit 072741a  
>Fix crash in OnViewModifySize()  
>Revert part of commit 4358753  
>Abort earlier in OpenSubStationAlpha with SRT input  
>Add NONE as option for subtitle renderer  
>Check pointer in GetImageFromMixer  
>Refactor some YDL code. Added option for preferred YDL audio format.  
>Avoid hang when aborting graph building. Instead terminate graph thread if it doesn't respond.  
>.  
>EVR-CP/Sync: allow 16-235 output range for full range input  
>Implement CWinApp::SaveAllModified  
>Fix WebVTT sub duplication issue with changing playback rate  
>Added an option to automatically save downloaded subtitles. (#1511)  
>1.9.22  
>Add support for DX11 subtitle rendering in combination with MPC Video Renderer (version 0.6.0 and above)  
- - - - - - - - - - - - - - - - - - - - - - - - - - - 


2022-05  
Contributor(s):  
adipose  
clsid2  
FoLLgoTT  
>Also set image duration for MPCImageSource filter. It currently ignores this API call, but hopefully support for it will be added in the near future.  
>Added advanced option for duration of images rendered by "Generate Still Video" filter. This is used for JPG/BMP images. Default is 10 seconds.  
>Improve long path support for command line parameters  
>Use relative paths when saving MPCPL playlist file  
>Fix video only button on Formats option page  
>Fix regression with moving subs up/down  
>alternate fix for crashing file dialog (#1702)  
>Added automatic reload of subresync data for embedded text subs  
>Fix crash on playlist "save as" when visual styles are disabled in windows 7 compatibility settings.  
>Added raw position and zoom values to /pns command line parameter (#1687)  
>Small tweak in source filter priority  
>avoid cfiledialog crashes when themes disabled (#1707)  
>fix for opening 260+ length audio file (#1717)  
>Improve code for overriding image display duration  
- - - - - - - - - - - - - - - - - - - - - - - - - - - 


2022-04  
Contributor(s):  
adipose  
clsid2  
>Add extra input mediatypes for MPCVR/EVR to allow connection with MPC Image Source Filter  
>Handle D3D same as normal in CreateFullScreenWindow()  
>Sanity check on AB-Repeat command line values  
>Abort early when parsing invalid xchp file  
>Fix mainframe controls in D3D mode  
>fix disabled buttons on favorites dialog (#1664)  
>fix bug with black bg in classic theme (#1663)  
>implement CModelessResizableDialog as wrapper for handling peek preview (#1649)  
>Revert skipping initial fullscreen state. Fix check to prevent exit of fullscreen.  
>clean up code for modeless dialogs (#1654)  
>Don't process standard PCM audio with LAV Audio Decoder  
>Clear mainframe window when switching to D3D fullscreen  
>Also allow inaccurate fast seek for small jumps when that jump size has been changed to 5000ms or more  
>Save playlist more often. Also delay writes in case of multiple changes in short timeframe, like when appending a selection of files through Explorer.  
>Fix issue with playback rate changes for external subtitles with VMR9  
>Make switch to D3D fullscreen a bit more reliable. It can still randomly fail to switch to  fullscreen, not showing anything. Not sure why yet.  
>support advanced option to force short menu when main menu hidden (#1674)  
>fix non-saving of favorites (#1669)  
>Unmute when setting volume through command line  
>add translatable strings for advanced options (#1686)  
>Skip unneeded playlist save when clearing it just before adding  
>Support A-B repeat per file/DVD.  Fix DVD support for A-B repeat (#1655)  
>reload ab repeat after sleep/pause (#1672)  
>dsf  
>disable redraw of favorites while rebuilding list (#1671)  
>Fix bug with EVR where it did not unload properly with a 2 frame 0.2 fps video sample.Plus some minor cleanup.  
>The "File(s)" context menu option on the Formats options page now controls whether the "Add to Playlist" context menu entry is created.  
>1.9.21  
>Add locks in SetD3DFullscreen()  
>More improvements to favoriteorganize dialog (#1645)  
>.  
>Remembered position was not properly reset at end of playback  
>Don't block options dialog if D3D fullscreen on other monitor  
- - - - - - - - - - - - - - - - - - - - - - - - - - - 


2022-03  
Contributor(s):  
Hugo Carvalho  
adipose  
clsid2  
>Small fix for commit ec74dba  
>Group together video and fps info in statusbar  
>Allow to disable reload after hibernate.Also disable reload by default.  
>update draw code for CFavoriteOrganizeDlg (#1632)  
>Some modifications to BEST_FIT algo for subtitles.  
>Check for nullptr in CWebClientSocket::ParsePostData()  
>Fix subpic texture size issue when switching to fullscreen exclusive with MPCVR  
>Add fallback for getShortHash()Simplify generating a hash for DVDs  
>Fix some compiler warnings  
>Fix langid for pt_pt  
>support fullscreen to second monitor while showing mainframe (#1614)  
>support extended locale names for subtitles (#1586)  
>Avoid unneeded calculations in CSubPicImpl::GetSourceAndDest()  
>Add advanced option RedirectOpenToAppendThreshold  
>Fix video display issue if fullscreen monitor setting was changed while being in fullscreen on another monitor  
>Skip going into initial fullscreen state when launching file in fullscreen  
>Missing part of previous commit.  
>Support all renderers in the newly added fullscreen ability where controls are shown on another monitor  
>Fix issue with subtitle PAR compensation  
>Fix subtitle texture regression when using ass after pgs  
>Disable redundant call to SetupChapters()  
>remember last quickopen and saveimage paths separately (#1582)  
>improvements to CFavoriteOrganizeDlg and CMPCThemePlayerListCtrl (#1635)  
>Fix crash in ToggleFullscreen()Could occur when player was started in fullscreen.  
>Improve BEST_FIT algoritm for subtitles.Fixes some issues with PGS.  
>Check m_pVW pointer in ToggleFullscreen  
>Fix another issue with PGS subs  
>Don't reset remembered position when beyond 95%  
>In case of multiple player instances, the first entry in the rfe_array is not necessarily equal to the currently open media. So store hash of current open media and use that to find corresponding entry in the array.  
>Fix regression regarding auto-hide of toolbar  
>Add advanced option for the new separate fullscreen window functionality  
>Support having two filters in graph that support IAMMediaContent interface  
>Re-organize audio renderer list in the options  
>.  
>1.9.20  
>Adjust commit ec74dba to apply only for EVR based renderers.Code is not working yet with other renderers.  
>Fix issue with creating preview renderer in case D3D fullscreen is enabled  
>Drop ACTIVATE and SETFOCUS window messages while closing player.  
>Fix issue with PGS subs  
>Improve Portuguese (Portugal) translation (#1624)  
- - - - - - - - - - - - - - - - - - - - - - - - - - - 


2022-02  
Contributor(s):  
Speeder  
adipose  
clsid2  
j2oc  
>Unreg file assoc during uninstall  
>workround for windows 11 bug with RTL in options (#1558)  
>Reset remembered playback position to begin if beyond 95% of duration when closing the file. Assume playback has reached the end credits.  
>Fix bug with clearing history when using INI  
>Fix some issues with saving playback position.  
>use individual registry keys for file history data (#1384)  
>Fix option "Prevent minimizing the player when in fullscreen on a non default monitor"  
>Revert commit 12476adb093a12811e9353e6fd24e5c702b44f6b.  
>Fix resource updater reading mpc-hc.rc with the incorrect encoding when it is encoded as UTF-8 without bom (#1512)  
>Purge invalid recent file entry in ReadMediaHistory  
>Simplify previous commit  
>Simplify some code for recent file menu.Filenames in MRU should never be empty.  
>Fix some issues with setting subpic texture size and allocation  
>enable peek preview fix for win10 as well (#1569)  
>Store lastopened value in RecentFileEntry.Use multimap for sorting hashes, as the lastopened value is not necessarily unique.  
>Use comma in xchp timecodes, similar to SRT timecodes  
>Fix regression of storing recent Dub list  
>Use float values in settings dialog for subtitle font spacing and border width/shadow  
>Check pointer  
>Prefer default sub over forced  
>Show MediaInfo details for the currently playing title on a DVD  
>Fixes for recent file code  
>Support loading chapter metadata from .xchp file (#1257)  
>More improvements for commit 1b7082f  
>Repaint in stopped state  
>.  
>Increase default number of remembered files to 100.Limit audio dub history count to 20.Limit number of recent files shown in file menu to 50.  
>workaround windows 11 peek preview bug with subtitle dialog (#1564)  
>Fix issue with adding YDL processed URLs to recent file history.Use the source URL for hashing instead of extracted URL.  
>Don't re-read history when clicking on a recent file entry  
>Improvements for commit 1b7082f  
- - - - - - - - - - - - - - - - - - - - - - - - - - - 


2022-01  
Contributor(s):  
Speeder  
clsid2  
>Always apply subtitle color correction  
>Don't update default playlist file on each modification during a session. Should speed up appending. It is redundant because the default playlist gets updated already when closing the player.  
>Updates for internal LAV Settings handling  
>Fixed audio capture info not initializing at startup (#1506)This issue caused CMainFrame::BuildGraphVideoAudio not to be able to read the audio media type, so it did not call SetLatency correctly.  
>Validate video size in CPlayerCaptureDialog::UpdateMediaTypes() (#1504)  
>1.9.19  
>Disable unused code in ColorConvTable  
>Increase time threshold for redirecting an "open" action to a "playlist add".Explorer creates multiple instances when playing/opening a selection of files. We attempt to redirect all actions to one instance if they happen in quick succession.  
>Properly handle no-break space in subtitles  
>Vista is no longer supported  
>Improve writing UTF8 data.Fixes issue when input had multibyte characters.  
>Add option to not open the recording panel automatically when opening a device (#1505)This is useful when just watching TV over a recording device  
>Some fixes for previous commit  
>Avoid creating new instances when add to playlist fails  
>Update copyright year  
>Check pointer of m_pBA. Move some code out of loop.  
>Only draw opaque box when either border or shadow is non-zero.  
>.  
>Update ydl_whitelist  
>Add BT.2020 support to ColorConvTable  
>Fixed device configuration persistence when its name contains backslash (#1500)  
>Enable more formats in internal source filter  
>Add hotkeys to zoom window size (#1501)  
- - - - - - - - - - - - - - - - - - - - - - - - - - - 


2021-12  
Contributor(s):  
clsid2  
RadarNyan  
>Let the Pan&Scan reset function also reset the subtitle position/size override.  
>Shift subtitle with 2px increments  
>Fix for commit 662812  
>Add setting values to adjust vertical position and font scaling of text based subtitles. These values can not yet be modified.  
>Fix lagging framestep with madvr  
>Refactor sub reset  
>LAV Filters 0.75.1-17-g8d177  
>Handle duplicate call to SetSubtitle()  
>Make sure subtitle size changes are visible directly  
>Increase max size of downloaded sub from 1MB to 64MB  
>Add yOffsetInPixels parameter to Subpic. Correct default value of xOffsetInPixels.  
>Lower minimum seekbar preview size from 10% to 5% of screen width  
>1.9.18  
>Handle text input that only uses CR as line breaks  
>Change date formatting for Logitech LCDSupport display  
>Add default hotkey values. Ctrl+Add/Substract for subtitle font size. Ctrl+Shift+Add/Substract for subtitle position.  
>Fix subtitle size/position issue after switching from PGS to other sub format  
>Don't use seek preview when a playing file from an optical disc  
>Add long path handling in a few more places.  
>Add hotkeys to move subtitle up/down and increase/decrease text size  
>Workaround for horizontal scroll with pen tablet (#1457)Horizontal scroll using pen tablet produces "Encountered an improper argument." error message. Note this doesn't make horizontal scroll with pen tablet work, just omits the annoying error messages.  
>Tweak behavior of speedstep setting. Now uses increments of 5% instead of 10%. Max is now 75%.  
>Disable safezone handling for seekbar when using madvr  
>Improve Subtitle::GetSubFileNames() to support long filename handling in Windows  
>.  
>Revert "Handle duplicate call to SetSubtitle()"This reverts commit e6e5c17b7524518acb76182f8b355d4e46057632.  
>Additional fix for previous commit.  
- - - - - - - - - - - - - - - - - - - - - - - - - - - 


2021-11  
Contributor(s):  
adipose  
clsid2  
>Increase threshold to 30min for file reload after long pause.  
>decode more utf-8 strings to utf-16, including emoji and 4+ byte utf-16 (#1376)  
>Add advanced option for controlling file reload after long pause  
>Remember current audio and subtitle track selection when doing automatic file reload  
>Use cached filter state in OnUpdatePlayerStatus  
>Optimize timer coder that is used for hiding mouse pointer and toolbars. Use lower timer resolution. Set timer for hiding toolbars only when a delay is applied, because UpdateToolbarsVisibility() is already called when moving the mouse, so no timer is needed in that situation.  
>Fix ReloadAfterLongPause setting  
>Add option to block RDP Redirection Filter  
>Improve seekbar behavior for very quick drag actions  
>Check pointers in SetPreviewVideoPosition  
>Fxi drawing of modern seekbar after commit 5705b1  
>Fix unintentional lowercasing of filename when adding to playlist  
>Disable SMTC by default for now  
>1.9.17  
>Fix regression with opening a rarset through command-line with path ending with "*.rar"  
>Fix bug in SearchInDir when there is just one file left in folder after the previously played file was deleted  
>Fix handling of 4-byte UTF8 chars  
>Improve name of the Windows version that is displayed in About box.  
>Fix for previous commit. Timer is also needed if HideWindowedControls is enabled.  
>Check pointer  
>Allow wildcards in filenames in M3U playlist.Some improvements to code that deals with adding files from folder or wildcard filename.  
>The seekbar thumb now follows the mouse pointer when dragging and only jumps to final seek position after drag ends. Previously it would jump to the actual seek points during the drag. This could give a jumpy experience for files with large keyframe gaps or short durations.  
>Add Portuguese translation to installer  
>.  
>improve utf-8 comments in textfile.cpp (#1373)  
- - - - - - - - - - - - - - - - - - - - - - - - - - - 


2021-10  
Contributor(s):  
adipose  
clsid2  
lifegpc  
>Add advanced option for use of System Media Transport Controls  
>Add CLSIDToString function  
>Add more clsid values to fallback method in IsVideoRenderer()  
>Improve D3DDevice locking code  
>Remove unused entry in list on keys options page  
>Improvements and refactoring of Media Transport Control code  
>Fix bug in safezone handling  
>Speedup generation of thumbnails image by seeking to keyframes when possible  
>Small fix for getting sub language in case it has no LCID  
>Improve subtitle track selection based on language preference  
>Fix subtitles being early by a few frames with VMR9  
>Store sub language name in addition to LCID. This is useful for languages that do not have a LCID value.  
>Only repaint video in paused state.Otherwise race condition can happen while closing a file.  
>use HTCAPTION for clarity (#1341)  
>Add support for System Media Transport Controls (#1339)  
>Additional improvements for previous commit  
>Some tweaks in FGManager to show more details in trace log.  
>Rewrite CAudioSwitcherFilter::CheckMediaType()  
>Check for nullptr  
>support dvb symbol rate (#1366)  
>Added advanced option to disable automatic pause when locking the screen  
>Improve inline edit (playlist) (#1332)  
>Reload file after long pause or hibernate  
>Use an explicit list of mediasubtypes when creating a video renderer instance. This helps avoid pointless filter connection attempts and renderer object creations.  
>Ignore "encode out" pin of Cyberlink video decoder  
>Check pointers in CPlayerCaptureDialog::SetupVideoControls()  
>Support "off" in subtitle track preference. It may follow language codes that you do prefer to use. In that case it will disable subtitles only if no preceding language code has matched.  
>paint background of videowindow in preview (#1359)  
>.  
>Fix point mapping bug for some uses of WM_NCLBUTTONDOWN event.  
>Allow all parsers for downloaded subs  
>Improve hooking code for ReceiveConnection/NewSegment/Receive functions to better handle multiple renderer instances. First instance now get the hook. Other instances will no longer mess with an active hook. Also unhook properly when destroying the renderer.  
>force tooltip to show up if mouse x position has not changed since losing focus (#1350)  
- - - - - - - - - - - - - - - - - - - - - - - - - - - 


2021-09  
Contributor(s):  
CW2  
adipose  
clsid2  
>Remove "allow bitstreaming" option for SaneAR. It is now always allowed.  
>update some code to help with future memory leaks. (#1279)  
>Use correct name for SaneAR in filters menu  
>Highlight default menu items in DVD audio and subtitle submenus. (#1276)  
>When the global after playback event is set to "play next file in folder", the player will now close the current file when no next file is available. This allow the screensaver to activate.Closing file now also exits fullscreen.  
>Catch exception in CTextFile::FillBuffer()  
>Enable play/pause command in closed state. This now also allows starting playback by pressing spacebar or clicking on the video area when playlist is not empty.  
>Fix crash when using old external LAVFilters as internal  
>Allow storing AB-repeat markers when saving as favorite (#1298)  
>port of mpc(be) audio renderer (#1239)  
>YDL skip local url  
>Support sending subtitle delay adjustments to VSFilter/XySubFilter (#1300)  
>SaneAR: Don't require exclusive mode to be enabled to allow bitstreaming. Bitstreaming will now automatically use exclusive mode.  
>Check nullptr  
>fix for synchronized behavior between output and audio renderer page (#1278)  
>support running scan when dvb stopped (#1312)  
>Copy favorites to clipboard (#1286)  
>Small change in dx9subpic  
>Some visual improvements to playlist numbering and title editing (#1331)  
>port some DSUtil and other utility code from mpc-be (#1243)  
>Fixed regression caused by bb316a. It broke drag&drop of folder onto playlist.  
>Another small tweak for media state cache  
>1.9.16  
>Improve media state caching code.CMainFrame::GetMediaState() needs to be declared as const to avoid a performance issue. This was noticeable when framestepping with madVR. I am not sure why exactly.  
- - - - - - - - - - - - - - - - - - - - - - - - - - - 


2021-08  
Contributor(s):  
CW2  
adipose  
clsid2  
>Fixed repeat of short animated gif files. Minimum duration is now reduced to 100ms to allow repeat.  
>Fix for getting the video fourcc in case of source filter and splitter are separate filters  
>Split HookNewSegmentAndReceive into two functions  
>Clicking the systray icon with mouse middle button toggles play/pause (#1267)  
>Replace DebugBreak with ASSERT  
>Pass settings to SettingsNeedResetDevice()  
>Fix some compiler warnings  
>Cache pointer to IAMNetworkStatus interface  
>Add option to show video fourcc and resolution in status bar (#1270)  
>Allow using environment variables in YDLExePath  
>AudioStreamSwitcher: Ignore NewSegment calls on inactive pins.This might solve a hang issue on seek or stream switch.  
>Add advanced option to specify location of Youtube-DL executable  
>Don't pause automatically when jumping to next/previous subtitle using hotkey  
>fix for default preview rotation on renderers that don't support rotation (#1271)  
>Remove code specific for old and obsolete external ogg splitter  
>Fix youtube-dl not being used when dropping URL onto playlist.  
>Fix saving of YDLExePath setting  
>Remove support for ffdshow frame queue option.That option is disabled by default in ffdshow, and I also consider ffdshow decoder to be inferior and obsolete.HookReceive is now only needed by VMR9.  
>Also show original URL in file properties dialog for YDL processed URLs  
>Sanity check in UnlockD3DDevice()  
>OpenSubStationAlpha: abort earlier on invalid file  
>optimize includes for cmpctheme, fix for cmpcthemegroupbox fonts (#1241)  
>1.9.15  
>Disable subtitle upload functionality. It is not working.  
>Put quotes around ydlexe.Show error message when process creation fails.  
>Support loading yt-dlp.exe from MPC-HC folder. YT-DLP is a fork of and alternative for youtube-dl.  
>Override CFileDialog::GetNextPathName() implementation and increase max path  
>Rename HookWorkAround10BitBug to HookReceiveConnection  
>Cache media control state  
>Add advanced option to show A-B repeat marker times in statusbar (#1263)  
>.  
>Partially revert c5734fc  
>Fix small issue with ydl processing  
>Small fix in CMainFrame::OnStreamAudio  
>Some improvements in file skipping code.Ignore if playing an URL.Fix issue with skipping after deleting current file.  
>Fix parsing of subtitle files that contain a duplicate UTF Byte Order Mark  
- - - - - - - - - - - - - - - - - - - - - - - - - - - 


2021-07  
Contributor(s):  
Gary Wang  
adipose  
clsid2  
>Improve GetOpticalDiskType function. Check if drive contains a disc before searching for files. Fixes long delay when opening File menu on some systems.  
>Small menu fix for large number of favorites  
>fix: draw repeat marker before chapter marker (#1198)  
>Revert a part of commit fbbaaff  
>improve evr-cp preview handling (#1190)  
>Some small tweaks  
>use zero as default (#1212)  
>Some tweaks for EVRCP in preview  
>Enable use of EVRCP for the seek preview  
>Check nullptr  
>Adjust DrDump preprocessor definition, so that only official builds use it.  
>Catch some file exceptions in vobsub code  
>Improve usage of GetCurFileName()  
>initialize defaultVideoAngle variable (#1197)  
>Fix manual subtitle search when playing online stream  
>1.9.14  
>.  
>Some improvements for when using EVRCP for preview  
>Remove streampospoller_active variable  
>Disable EVRCP for preview until a bug is fixed  
>Fix issue where EVRCP preview messed up subtitles  
>Fix regression since 1b8d60 that broke jumping to end of file  
- - - - - - - - - - - - - - - - - - - - - - - - - - - 


2021-06  
Contributor(s):  
adipose  
clsid2  
Faberman  
>Add external fonts support (#1140)  
>Don't override blocked AudioSwitcher in case of BDA graph  
>Don't seek to keyframe for videos shorter than 1 minute. Always doing an accurate seek is preferred in that case.Tip: for longer videos you can perform an accurate seek by holding Shift while clicking on the seekbar.  
>Fix wrong url for OpenSubtitles  
>Reduce minimum seekbar drag distance for triggering a seek  
>Don't show seek preview for still image. Skip some preview code when there is no known duration.  
>Small tweak in mediatypes for NullTextRenderer. Based on patch by MPCBE.  
>Improve chapter jumping when first chapter is not at start position  
>Some tweaks in graph builder. Avoid making certain filter connections when we already know it is going to fail.  
>Remove useless list of predefined subtypes for "add subtype" in external filter settings.  
>enforce safe zone on mousetouch earlier. (#1183)  
>Fix bug in e4cba0 that broke skipping to next file if file had a title  
>Proper fix for blocked AudioSwitcher  
>Check for nullptr  
>Store pointer to IAMMediaContent interface. Optimize code to perform EnumFilters loop less often.  
>1.9.13  
>Modify DeleteFileInPlaylist to update playlist before deleting file.  
>Some tweaks in framestepping code  
>Revert "Use a copy instead of a reference for RenderersSettings in the render threads"This reverts commit 0419938469f03475d6568d5fc82680eab1da9ac4.  
>use evr-cp for preview to support rotated videos (#1176)  
>Some more tweaks in graph builder  
>Handle <br> tag in srt subtitles  
>Installer: remove web shortcut in Start menu  
>Ignore repeat in case of still image file  
>Fix issue with launching in fullscreen with D3D Fullscreen exclusive mode option enabled  
>Fix for preview commit.  
>enable safezone for seekbar (#1171)  
>Some improvements to the subtitle texture code. Also fixed a hang issue when texture allocation failed.  
>Allow repeat for animated gif, if duration is at least 1 second.  
>Fix regression with loading audio dub from command line  
>Add assert in CMainFrameControls::ShowToolbars  
>Small tweak for queuedSeek usage  
>.  
>do not expand no-click zone near seekbar horizontally (#1177)  
>Revert "Revert incorrect changes to RLE subtitles (#1112)"This reverts commit 9e30590b274efed9775fac059425529a31d0cbc9.  
>Check for nullptr.Silence compiler warning.  
>Tweak handling of video renderer failure.  
>Improve DVD region code bypass  
- - - - - - - - - - - - - - - - - - - - - - - - - - - 


2021-05  
Contributor(s):  
Kacper Michajłow  
clsid2  
lifegpc  
>Set AddLangCodeWhenSaveSubtitles to false by default  
>Small tweak in OSD code  
>Check for nullptr in CPlayerSubresyncBar::ReloadSubtitle()  
>1.9.12  
>Improve IAMOpenProgress related code  
>Fix ydl processing when adding multiple URLs using /add command line parameter  
>Fix some compiler warnings  
>GetRemoteControlCode: Fix invalid memory access (#1109)  
>Small changes  
>Some improvements in GetContentType()  
>Fix osd regression caused by 5236405  
>Subtitle download support for youtube-dl (#781)  
>Small tweak in CFGManager::ConnectFilter  
>The font "Splatter" that is used in some anime videos renders very slow with GDI. Replace it with Arial as a workaround to get normal subtitle rendering performance.  
>Strip <span> html tag when found in SRT subtitles  
>Fix crash with empty SSA collisions script tag  
>Use a renderer backbuffer size that is equal to screen size by default. Added a (hidden) option to get old behavior of using desktop size for windowed playback.  
>madVR: Fix IMadVRSubclassReplacement usage (#1108)It needs to be called after madVR is created when the interface isactually available.  
>Improve AVX2 check  
>Modified maximum subtitle texture setting.DESKTOP choice has been removed because that default setting could give bad performance on systems with UHD screen.VIDEO choice has been removed because that could give bad quality. It also simplifies the code.New default setting is a maximum texture size is 2560x1440. So for any screen up to that resolution there is no difference in behavior compared to DESKTOP choice. In case of UHD and the default 1440p setting, a texture of 1080p size will be used to get a nice scale factor.When video window size exceeds the maximum texture size, it now calculates a new texture size with same total pixel count but with AR of video. Previously it would limit size based on the max width/height, giving lower quality than desired.  
>Add .weba extension to formats list  
>Fix regression in subpic texture code. Calculation could overflow, giving wrong result.  
>Ignore .cue files when drag&dropping a folder  
>Show prompt about requiring a login when enabling OpenSubtitles.Based on patch made by kenny1983  
>Check nullptr in OnGraphNotify()  
>Fix opening of audio dub from file menu  
>Fix crash in DoMessageBox()  
>Some tweaks to ydl sub code  
>Disable ydl subs by default  
>Set renderer backbuffer size equal to largest monitor. Fixes regression when dragging video to a monitor with larger resolution than initial one.The (hidden) option DesktopSizeBackBuffer still allows to use whole desktop as backbuffer size.  
>Removed non-functional SubDB subtitle search provider.Removed code from older non-functional providers.  
>Re-order list on advanced settings page  
>Fix issue with unwanted dots being displayed in front of extensions on Formats page after changing selections  
>.  
>Revert incorrect changes to RLE subtitles (#1112)* Partially revert "Render subtitles relative to the window by default."CRLECodedSubtitle (DVB/PGS subtitles) have to be drawn relative to video. Best-fit logic was specifically designed to handle those subtitles in various cropping scenarios and sizes.This partially reverts commit d37ea5aa08d0df5cda965404dbf5d970ea45133b.* Revert "Improve PGS subtitle positioning (#172)"The fix was incorrect.This reverts commit d6962bd1ac2dc66c584c8e52279ebb745749639c.  
>More tweaks in GetContentType()  
>Removed obsolete RealMedia and QuickTime framework code, which was only used in 32-bit builds. Now it will always use the internal DirectShow filters for those formats in 32-bit builds, just like the 64-bit builds have always done.  
- - - - - - - - - - - - - - - - - - - - - - - - - - - 


2021-04  
Contributor(s):  
t3chnical1  
lifegpc  
adipose  
Andrey Bazhan  
clsid2  
>Fix compilation after previous commit  
>Update inno setup custom messages translation file (#1036)  
>Add HTTPS support for web playlists (M3U, PLS, RA, ASX etc.)  (#1032)  
>After long pause (> 10 min) or hibernation, force a seek to current position. This re-initializes the video decoder, and hopefully solves a hang/crash on certain systems.  
>Some tweaks in GetContentType()  
>Some tweaks in CPlayerPlaylistBar::ParsePlayList  
>Fix issue with recent file list getting cleared due to files with long path being handled wrong.Simplify recent file code.Fix issue with drag&dropping folders that exceed MAX_PATH.Optimize updateRecentFileListSub() to avoid unneeded updates.  
>Regression fix in GetContentType()  
>Small tweak in OnPowerBroadcast  
>Fix a71a144  
>Reset Pan&Scan settings when closing media, except when option "Remember last Pan & Scan Zoom" is enabled.  
>Small optimization to skip OSD code when disabled  
>fix fullscreen launching and restoring issues (#1075)  
>Better error message for when essential DirectX file is missing.  
>WebVTT color style parsing fixesPatch made by pinterf  
>Minor changes  
>Don't reset playlist when opening a file from favorites if that file already is in the playlist (#1078)  
>Support DASH manifest URLs extracted by YDL.Fixed bug when extracted URL is same as input URL. This caused endless re-processing by YDL.  
>fix ontop bug with fullscreen pause (#1074)  
>fix for unapplied styles and color regex (#1054)  
>Initialize value of m_dwLastPause  
>improve webvtt style code (#1049)  
>Check for nullptr  
>Check access permissions for settings INI file itself if the file exists. Only check if whole folder is writable when it doesn't exist.  
>Skip menu setup code when main window is already destroyed  
>Timeout after 250ms in LookupSubPic when no subpic is available.  
>Possible crash fix in subtitle task code  
>Fix issue with jumping to wrong chapter in certain multi-segment mkv files  
>Fix parsing of a SSA subtitle that has wrong scripttype version  
>1.9.11  
>Change setting storage name of Subpic Queue Size, to reset to new default value on existing installs  
>support webvtt style blocks: foreground and bg colors (#1026)  
>wingdi GetPath can return -1  
>Add ability to quickly save playlist (#1076)  
>Remove unneeded call to SetupChapters  
>.  
>Improve jumping to previous/next chapter  
>Fix small memory leak in SubtitlesProvidersUtils::GenerateOSHash  
>Fix building installer  
- - - - - - - - - - - - - - - - - - - - - - - - - - - 


2021-03  
Contributor(s):  
adipose  
clsid2  
lifegpc  
>improvements to webvtt (#1023)  
>update audio renderer when checking internal AR checkbox (#980)  
>Show the time of the displayed frame as tooltip text in the preview window, instead of the exact time at which you are pointing with mouse. Tip: holding down Shift allows the preview to show non-keyframes, and the tooltip will show the corresponding time as well.  
>Support embedded WebVTT subtitles  
>Fix handling of embedded WebVTT when it comes as a single blob of data  
>Fix crash if display mode list is empty  
>Fix regression for repeat forever radiobutton after f437d40.Also remove unused variable.  
>Fix compiler warning  
>Small tweak in SubPicQueue  
>Improve WebVTT tag stripping  
>Don't show title in recent file list if title is included in filename. (#1025)  
>mpcvr is compatible with xysubfilter  
>Add comment about linear sampling in pixel shaders  
>Revert 8c029b3. It caused an issue were subs could get skipped.  
>Add some extra state tracking variables for menu creation and media opening  
>Check for null pointer  
>Simplify code for disabling screensaver.Remove API call that was only relevant for XP and older.  
>Check for nullptr  
>fix regex for foreground only color webvtt fonts (#992)  
>Fix SVG toolbar not loading with certain scale factors.  
>Add a small delay when opening media in case the filter graph for previous media is still loading. This should improve stability, because aborting graph creation can sometimes hang. Waiting a short time can potentially avoid the need to abort the graph.  
>Fix seeking to nearest keyframe  
>use utf-16 for rc file (#971)  
>don't trim ASS dialog style namesbased on patch from xysubfilter  
>Minor changes in subtitle code  
>Suppress warning about unknown styles in ASS subtitles  
>Set video size in the subtitle renderer before it starts parsing the data  
>force redraw of items if item removed (#989)  
>don't show preview when using 'on top' (#981)  
>Remove/replace tags when parsing embedded WebVTT.  
>Also strip align tag  
- - - - - - - - - - - - - - - - - - - - - - - - - - - 


2021-02  
Contributor(s):  
adipose  
clsid2  
lifegpc  
>Fallback to ansi encoding if invalid UTF8 data encountered when reading (playlist) text file  
>Revert part of previous commit. It caused a crash regression in classic theme.  
>Add checkbox for enabling internal audio renderer (#935)  
>show shader name only in combobox in debug shaders (#888)  
>Disable sub picture buffer by default  
>More detailed subpic debug trace logging  
>Fix bug in SubPicQueue.rtCurrent could be wrongly set to smaller value.  
>@adipose common themed tooltip controls for dialogs.  (#905)  
>close GDI memory leaks (#874)  
>Add some missing calls to ReleaseDC() and DeleteDC()  
>Catch memory exception in InitInstance  
>Don't invalidate subpic queue at start, it is already empty  
>Renamed "System Default" video renderer to "Video Mixing Renderer 7". This to prevent people from thinking this ancient renderer is the preferred one. Also changed order of the available renderers to put the recommended ones at the top.  
>remove unwanted lines from STS (#936)  
>1.9.10  
>Use title to replace file name if video is a youtube-dl source URL. (#902)  
>ydl: show progress in console title and show error message if it fails to run  
>Increase max preview window size to 40%  
>update translation resource management script to support Python3 (#878)  
>Fix issue with opening online .asx playlist  
>add strings for advanced options (#923)  
>Fix  bug with comparing double value with 0.0 which sometimes failed.  
>Fix regression in savethumbnails. Seeks should not use delay.  
>don't show preview while dragging (#917)  
>1.9.9  
>support simple webvtt color tags (#930)  
>Some optimizations for preview code. Fixed issue that showed wrong keyframe in preview. Now a seek will go to same frame.  
>Ignore window messages when main window is already destroyed. Should fix a crash issue with madvr when closing player.  
>Webserver: no need to read mimetypes from registry. Own list seems sufficient.  
>Fix bug in keyframe search code. An exact match was not handled correctly.  
>.  
>Clean some disabled code  
>Add d3dx9 header files to repo. This eliminates the need to install the old DirectX SDK, which is no longer available for download.  
>Fix regression regarding handling subtitle text encoding  
>Don't use deinterlace filter when capturing if biWidth in BITMAPINFOHEADER is zero.  
>Seeking improvements. Enforce a small pause between consecutive seeks. Also prevent building up a queue of unprocessed seek events when holding down the arrow buttons. Instead when seeking is slow, excessive seek events are skipped.  
>convert mpc-hc.rc to UTF-8 (#924)  
>Check return value in BeginEnumSysDev macro  
- - - - - - - - - - - - - - - - - - - - - - - - - - - 


2021-01  
Contributor(s):  
adipose  
clsid2  
MSchif  
lifegpc  
>do not auto-zoom dvd video when player is maximized (#828)  
>Remember default rotation so that PnS reset/rotate works properly  
>preview: fixes for VMR9; compile fix for lite versions (RAR) (#866)  
>Fix language display in status bar (#859)  
>Add nullptr check  
>Fix a few compiler warnings  
>Simplify keyframe code for preview graph  
>support showing fps / rate on status bar with advanced option (#830)  
>Fix some compiler warnings  
>Don't show title in recent file list when it is similar to the filename (#837)  
>Fix compiler warning  
>Fix issue with LAV settings window not appearing immediately after using seek preview.  
>Tiny fix  
>Fix bug when label name is equal to file name in recent file list. (#827)  
>Don't show title in recent file list when it is nearly identical to the filename  
>Disable crash reporter when closing player. Crashes that happen after this are mostly in third party code, so avoid bothering users with an error and just close.  
>improvements to preview window  (#855)  
>Avoid P010/P016 colorspace in preview window because of buggy EVR  
>Improve window title code  
>Reset audio renderer displayname if it is an invalid device  
>Trim whitespace from extracted title strings. Some files contain just a bunch of spaces.  
>Don't use HookWorkAround10BitBug for preview graph. Fixes crash with VMR9.  
>Only create preview window when that option is enabled.  
>Small fix for previous commit  
>Skip creation of preview graphbuilder for audio files  
>Another improvement in macrovision kicker code  
>Update copyright year  
>Improvements to OpenSubtitles uploads (#852)  
>Fix infinite loop in LastIndexOfCString  
>Small improvement in macrovision kicker code  
>Fix automatic metadata based video rotation with MPCVR as renderer  
>Removed some unused code from vd2 system project  
>updates to support c++latest (20) and default lang to cpp17 (#838)  
>.  
- - - - - - - - - - - - - - - - - - - - - - - - - - - 


2020-12  
Contributor(s):  
adipose  
clsid2  
lifegpc  
>Reset to default audio device in the settings if the stored device is no longer valid  
>Tweak version script to deal with situation where git checkout does not contain full history  
>Fix typo in function name  
>Finetune LAV settings for better seekbar preview performance  
>Add nullptr check  
>Small tweak to seekbar clicking behavior  
>Try to reprocess YDLSourceURL by youtube-dl if failed first time. (#786)  
>allow preview size to change dynamically. (#812)  
>Seekbar preview capability (#806)  
>add gaussian blur (#745)  
>Fix some compiler warnings  
>Improve display title for youtube-dl bangumi video (#775)  
>do not change enable/disable shaders when no shaders are available in the current preset (#773)  
>No need to clear OSD when closing graph immediately after  
>Insert null video renderer when rendering audio dubs. This ensures that no second video stream is rendered when a  dub contains video.  
>Fix bug in version generation script  
>Hide unsupported video renderers in the settings  
>Improve URL Location display in properties clip page (#787)  
>HookWorkAround10BitBug is not needed with MPCVR  
>Fix bug when parsing M3U file. (#804)  
>support YDL source url when parsing M3U file (#795)  
>Fix title can not update when streaming normal url. (#778)  
>ignore automatically created tooltips in subtitle dl dialog (#785)  
>Validate entries in mpcpl playlist (#793)  
>MPC-HC crash when opening BD by using Open Disc menu (#822)  
>Add the individual playlist entries to the recent files (#784)  
>Add IsURL function  
>Change minimum allowed playback rate from 0.125 to 0.05x  
>Improve URL Location display in properties clip page (#776)  
>Update DVD workaround for old video renderer  
>Rewrite Recent Files List (#757)  
>Some tweaks to YDL code. Added some parameters.  
>Fix missing & in recent file list (#790)  
>monitor color profile optionally applied to logo (#818)  
>When Youtube-dl fails to process an URL for a known website, then don't try to open the URL directly because we know that will fail as well.  
>Validate input in CSubPicAllocatorPresenterImpl::SetVideoSize  
>Read Playlist Info from youtube-dl (#780)  
>add NULL check (#794)  
>Do not combine path if it is a URL when parsing Playlist file. (#779)  
- - - - - - - - - - - - - - - - - - - - - - - - - - - 


2020-11  
Contributor(s):  
adipose  
clsid2  
lifegpc  
>shader toggle options (#719)  
>add cue sheet support (#725)  
>Small fix for sub lang in statusbar  
>Update LAV settings code  
>fix issue with themed menus showing accelerators appended to text (#720)  
>Increase default value for number of recent files that is remembered to 40  
>Added code to use libass for subtitle rendering. It is not yet active.ToDo: add option in GUI  
>Small tweak in status bar code  
>Fix some issues with M3U parsing  
>fix enable/disable code for shader menu (#739)  
>Show informative message when the video renderer fail to load  
>clear shaders by default to honor enable/disable setting.  store enable/disable setting permanently. (#747)  
>Improve code for ignoring fullscreen command line switch in case of only audio files  
>Fix MPC-HC crash when opening BD by using Open Disc menu. (#769)  
>support PerMonitorV2 for dpi scaling between monitors (options dialog) (#665)  
>fixes for mouse modifiers (#750)  
>Fix scripts to match transifex formatting (#738)  
>Catch exception from CFile::GetStatus/SetStatus  
>clear cache when calling setshaders (#742)  
>Reset rotation settings on file close  
>Finetune seekbar drag behavior  
>Check nullptr in CPlayerBar::SaveState  
>Some tweaks to video rotation code.  
>Revert "Remove redundant backslashes in bdmv path"It broke loading bdmv from UNC pathThis reverts commit 8c32030c8378951d5b265e8f9bae6e96cad6a44a.  
>Some more subpic texture tweaks  
>Allow entries in the recent file list to be a bit longer. Only shorten paths longer than 150 chars.  
>Do nullptr check again after GetDuration call. Race condition can trigger crash.  
>Fix crash with empty ass subtitle tag  
- - - - - - - - - - - - - - - - - - - - - - - - - - - 


2020-10  
Contributor(s):  
bacondither  
adipose  
clsid2  
lifegpc  
>Display new line char in Properties page description. (#713)  
>Add Option to remove language code when save subtitles. (#663)  
>Fix compilation with VS2017 (#694)  
>Fill lcid value in CVobSubFile::GetStreamInfo  
>Allow using LAV Filters with Lite builds. Now it only isn't build/included by default. This build config is mainly used for debugging, so useful to have minimal difference with standard builds.  
>Add nullptr check  
>support per window dpi differences (#646)  
>FIX regex_replace broke Unicode chars while parsing WEBVTT file. (#677)  
>Revert "Support resource specified font for StatusBar and InfoBar (#625)"This reverts commit 53c6ae973f01b4f3b086a8e008955678cdfa86bc.  
>Fix rare crash in CPlayerInfoBar::Relayout()  
>Optimize getting subtitle language for display in statusbar  
>Fixed bug that caused the SaveImage subtitle option to be shown when using madVR (which has its own option for that)  
>fix theme issues for MPC-BE AR (#696)  
>Some optimizations for WebVTT parsing  
>Set bShowLangInStatusbar to false by default  
>Finetuned the behavior when clicking/dragging the seekbar. This should fix an issue where two simultaneous seeks could be done, once of mouse down and once on mouse up, while only one should be made.  
>Only insert "File Source (Async)" in cadidate filter list when opening an existing file  
>Fix hang with madVR after commit 197c2fa  
>Fix saving of new mouse binding  
>1.9.8  
>Use Ctrl+MouseWheel as default key binding for large jumps (20s)  
>Fix hang with MPCVR if max subpic texture size was set to VIDEO.  
>Small bugfix for currentAudioLang  
>Add nullptr check in InitInstance to fix rare crash  
>Update remove tags regex when opening WEBVTT file (#668)  
>Partly support escape sequences in WEBVTT file. (#673)  
>Support ctrl, alt, shift modifiers for mouse (#671)  
>Add backup URL to update checker (#657)  
>Do not display HTTP GET parameters in title ,recent file list and properties page. (#623) (#661)  
>improvements to CMPCThemeMenu  (#707)  
>Ignore parameters behind #EXTM3U tag  
>Remove redundant backslashes in bdmv path  
>Avoid duplicate entry for "File Source (URL)" in candidate filter list  
>don't use FVIRTKEY bit flag for mouse virtual codes (#692)  
- - - - - - - - - - - - - - - - - - - - - - - - - - - 


2020-09  
Contributor(s):  
adipose  
clsid2  
>do not auto fullscreen for audio files (#617)  
>Support resource specified font for StatusBar and InfoBar (#625)  
>Reduce chance of crash in subtitle renderer when seeking and rapidly making screenshots at same time.  
>Support MPC(BE) Audio Renderer  
>show audio and sub language on statusbar (#627)  
>Add advanced option for displaying the languages in statusbar  
>Shorten long entries in recent file list  
>Fix for temporary status messages.  
>Fix for previous commit  
>Fix regression with opening from recent file menu  
>Fix compiler warning  
>Ignore MPC Audio Renderer when generating stream selection menus.  
>Removed RM/QT framework related options. These are irrelevant in 64-bit builds because they compatible with 32-bit only. For 32-bit builds we now just use sane defaults, and there is no need to give user ability change them.  
>Detect audio changes for status bar (#629)  
>fallback to 639-1 lang tags when finding LCID (#632)  
>support subtitle rate fix with downloaded subs (#619)  
>show decoded urls in recent files (#612)  
>Adjust text for language statusbar info. Unicode chars don't always work on Win7.  
>fix bug with convenience function to use type (#626)  
>Also update lang in statusbar when changing audio track using hotkey  
>Improve DVD region bypass  
>Add some logging for YDL processing  
>sanear: try to detect backwards frame jump caused by DVD still handling in ffmpeg and reslave (#611)  
>lock render and sub lookup while cache could be modified (#641)  
>add convenience function to get lcid string  (#624)  
>Fix for external subs (#630)  
>loop within folder after 'move to recycle bin' (#620)  
- - - - - - - - - - - - - - - - - - - - - - - - - - - 


2020-08  
Contributor(s):  
adipose  
clsid2  
>show non-exact filename matches if no exact match was found with search on OpenSubtitles (#600)  
>Don't sort chapter list by time. Adjust lookup function to handle unsorted list.  
>add Bulgarian language option (#579)  
>Revert " unset accelerators that conflict with user-specified ones  (#517)"This reverts commit c9681e171fedefa7bc2eeab900fe44ddb8332b3f.  
>fix some unthemed components (#562)  
>Fix bug where next file would be played after removing a file from playlist different than the currently played file.  
>Small tweak in OnOpenMediaFailed  
>Improve parsing of M3U files  
>fallback to search by title on opensubtitles.org (#566)  
>Fix issue where file could still be in use after closing.  
>restore lost code when fixing indent (#594)  
>Don't parse m3u playlist urls extracted with youtube-dl, but open them with lav splitter directly.  
>Changed default keyboard shortcut for subtitle upload to ctrl+u  
>Be less strict regarding file extensions when parsing subs.  
>Highlight duplicates in accel table (#563)  
>Updates for organize favorites (#567)  
>fix indent in SubPicQueueImpl.cpp (#592)  
>Change default keyboard shortcuts for DVD menu navigation to Ctrl+Shift+Arrow keys. This is too avoid conflicts with other shortcuts.  
>Fix subtitle search on Podnapisi for Portuguese Brazil language  
>Ignore invalid iso language codes  
>Open all M3U urls directly with LAV Splitter  
>Support URL and UTF8 decoding of streaming URL titles (#591)  
>Optimize hash calculation function for subtitle search  
>fix blinking with subtitles in certain edge cases when using sub queue (#588)  
>Check null pointer in UpdateThumbnailClip  
>1.9.7  
>Some tweaks to ParsePlayList and GetContentType  
>.  
>Try/catch does not work for access violation exceptions.This reverts commit 91145809540919903291bf3d33d011a1b3fe6305.  
>Fix typo bug in LAV settings code  
>fixed playback rates in menu (#570)  
- - - - - - - - - - - - - - - - - - - - - - - - - - - 


2020-07  
Contributor(s):  
adipose  
clsid2  
>Refactor theme setting code  
>avoid error with c++17 deprecation warning (treated as error) (#516)  
>use polib to match PO formatting to transifex (#509)  
>Some code simplifications  
>Reset window owner before releasing graph manager.Skipped in case of madVR because it doesn't work correctly with that.  
>libass support for VSFilter (#542)  
>Remove unused sub upload checkbox  
>Try more subtitle parsers for some extensions. Fixes regression with loading certain .sub subtitles.  
>Don't override background color for swf  
>Add fixme  
>Use a copy instead of a reference for RenderersSettings in the render threads  
>Catch rare null pointer exception in OnTimer. It's due to a race condition, but a lock doesn't work well here.  
>Refactor some code  
>Improve file delete code  
>unset accelerators that conflict with user-specified ones  (#517)  
>store libass settings in subrenderersettings (#530)  
>Fix race condition in ShowMediaTypesDialog  
>Improved drag&drop.Only load a file as subtitle if it has a known subtitle file extension.Show status message when subtitle fails to load.Ignore subtitle files when appending to playlist.  
>Fix parsing of timecodes of WebVTT subs. Hours value is optional, millisecond value is required.  
>Some cleaning  
>Add missing bitstreaming fallback option to internal LAV settings management.  
>Revert "use polib to match PO formatting to transifex (#509)"This reverts commit a5d660413c5f3989db11a9f483298284eee9e75c.  
>set settings before opening RTS; reset on style override (#533)  
>Clear stored path for temp file used during embedded sub parsing  
>Adjust flags for file open dialog  
>Move lock  
>Disable some unused code  
>Also support https in CWebTextFile  
>Use polib to format PO files for transifex compatibility (#523)  
- - - - - - - - - - - - - - - - - - - - - - - - - - - 


2020-06  
Contributor(s):  
adipose  
clsid2  
>Adjust initialization error messagebox  
>Prevent infinite recursion in ParsePlayList  
>Fix crash in OnTimer  
>Ignore SetSubtitle call when closing  
>dx11 shaders; code to select correct shader version for built-in shaders (#443)  
>Fix crash in CChildView::OnEraseBkgnd  
>.Signed-off-by: clsid2 <clsid2@users.sourceforge.net>  
>Catch file exception in pgssub  
>support flip and mirror for mpc-vr (#429)  
>Fix issue where wrong filename was displayed for subtitle search results  
>Revert code change that showed % on OSD. It caused heap errors when seeking (using remote control app).  
>1.9.5  
>1.9.6  
>prevent accidental drag when maximized, but allow long drag actions (#435)  
>allow afterplayback settings to be preserved (#505)  
>Update output settings page for added MPCVR capabilities  
>Recognize some more third party audio renderers  
>parse vtt before RT (#480)  
>Update filter blacklist  
>Catch file exception when opening subtitle file  
>Check DragQueryFile return value  
>support dx11 shaders in mpcvr (#440)  
>Update installer  
>Catch CFileException in VobSubFile::ReadSub  
>CDDAReader: some minor code re-ordering  
>Fix some warning and some cleanup  
>Round down displayed time instead of rounding to nearest second  
>Minor changes  
>Disable subtitles submenu when no video file is loaded  
>Properly save h264mvc setting from lav  
>Add advanced option for disabling crash reporter  
>Open crash report page  
>code indentation  
>support hwheel messages sent to main window for hotkeys (#477)  
>Remove some unneeded sse2 checks  
>Catch file exception in VTSReader  
>Enable crash reporter  
>Update DrDump code  
>Assign Delete as a default key binding for "Move to recycle bin" action  
>Improve coverart filename search  
>Enable SnapshotSubtitles option by default  
>Fix potential crash on file close  
>Fix crash when messagebox was used before settings were initialized  
>Better column widths for the table on the advanced settings page  
>Added advanced option to configure how often the time in the status bar is updated. This value is ignored in situations where a short interval is required internally, such as when SubResync is active or during DVD playback or Capturing.  
>Crash fix for changing subs through API with ISR inactive  
>Fix crash with long unc path  
>Mark overlay mixer as unavailable on x64 builds  
>support shaders/stats/rotate with MPCVR (#422)  
>More timer fixes  
>Another fix for output page  
>Fix crash in CPlayerInfoBar::OnEraseBkgndSigned-off-by: clsid2 <clsid2@users.sourceforge.net>  
>Some cleanup in About box  
>Emulate support for pre-size shaders on mpc-vr; add note to shader dialog (#448)  
>1.9.4  
>Enable renderer settings and shader menu items only when relevant  
>preferred subtitle parser ordering (#481)  
>Revert "support hwheel messages sent to main window for hotkeys (#477)"This reverts commit e7f4e8f4dfdca97a83f7c84d0d36aadd64d3a3cf.  
>.  
>Add code to use libassSigned-off-by: clsid2 <4704996+clsid2@users.noreply.github.com>  
>Remove lock from OnTimer. It caused hang issues when using remote control apps.  
>Fix crash in CDX9SubPic::CopyTo  
>support hwheel messages sent to main window for hotkeys (second attempt) (#495)  
>Revert madVR stats workaround from previous commit  
- - - - - - - - - - - - - - - - - - - - - - - - - - - 


2020-05  
Contributor(s):  
adipose  
clsid2  
>Compile fix for previous commit  
>Remove toolbar link from installer  
>OpenSubtitles currently does not support anonymous login anymore. So skip login (and search) if no account has been setup.  
>Fix regression with handling .pls playlist file  
>Fix DTV codepage issue for certain station names (#399)  
>force BDA Network Provider to Microsoft standard provider (#377)  
>Updated toolbar image and donate links.  
>port subtitle rate fix from mpc-be (#374)  
>1.9.3  
>Snapshot features (#387)* Use display aspect ratio when saving screenshots* Add advanced options to control snapshot video extension in filename and rendering subtitles* support drawing checkboxes on windows file dialog* add subtitles checkbox to save image dialog  
>Allow skipping past unopenable/closed/deleted files (#380)  
>Enable SubDB and podnapisi subtitle search providers by default.  
>Add variables.h (#390)  
>More fixes for snapshots (#398)  
>Revert old change that broke CDText  
>Use AAC generic uuid instead of LAVFilter specific one (#402)  
>fix CC and CCW rotate switched for EVR (#375)  
>Transifex updates (#424)  
>do not drag from logo area when maximized (#389)  
>Improve responsiveness in seeking when dragging seekbar thumb.  
>Show snapshot subtitle option only when supported by renderer  
>prevent hang while MediatypesDlg is open and new file is launched (#394)  
>scroll to make current channel visible on resize (#408)  
>Make subtitle rate change work with madVR and MPCVR as well  
>force explicit use of GetVideoSize() (#410)  
>Support AAC-ADTS in DVB (#401)  
>Fixes for VS2019 Build Tools v142  (#421)  
>Fix call to GetVideoSize() (#411)  
>Fix regression that could cause opening of a next file to fail  
- - - - - - - - - - - - - - - - - - - - - - - - - - - 


2020-04  
Contributor(s):  
adipose  
clsid2  
>Fix auto-zoom setting  
>Adjust subtitle search file extension list  
>show percent complete on OSD / status bar (#341)  
>Fix text encoding of opensubtitles login error  
>only show percent on OSD (#343)  
>Add .dv and .dav to supported file formats  
- - - - - - - - - - - - - - - - - - - - - - - - - - - 


2020-03  
Contributor(s):  
adipose  
clsid2  
>limit playlist submenu to 128 items (#302)  
>disable IME on main window to support hotkeys (#335)  
>support 25% zoom (#305)  
>Fix for internal audio render and DTV (#295)  
>A-B Repeat feature (#317)  
>Rewritten AB-Repeat code. Using SetPositions() function caused a short playback freeze when disabling B position. So instead of modifying the playback range in the DS filter graph we check if we need to jump back to A using the same timer as that is used for updating the seekbar position. This triggers every 40ms, so accurate enough imho.  
>Don't show login error for OpenSubtitles when username is empty.  
>Support basic VTT subtitles (#303)  
>Support mouse left/right wheel (#321)  
>Refactor DVB to BDA in DVBChannel.  Support ATSC tuner (minimal) (#322)  
>Ability to parse EXTM3U format (title only) (#314)  
>Optimized GetContentType function to avoid doing unnecessary file reads. This should give a big performance improvement when adding (lots of) files to the playlist.  
>do not scroll while updating pin edit window text (#331)  
>Fix for relative paths in M3U (#318)  
>decode utf-8 filenames properly (#334)  
>1.9.2  
>Change file art on each file if prio art is file specific (#320)  
- - - - - - - - - - - - - - - - - - - - - - - - - - - 


2020-02  
Contributor(s):  
adipose  
bfoos  
clsid2  
>check return value of createfontindirect (#256)  
>set madvr owner window after creating renderer (#258)  
>Also remove changelog file in installer  
>More  
>don't restore minimized window when adding to playlist (#281)  
>Crash fix [drdump  310801]  
>Transifex updates (#274)  
>support searching unicode with removed diacritics (#268)  
>Support lower case of other locales in key filter (#269)  
>Fix mpc-hc_setup.iss (#289)  
>Pan & Scan rotate options in menu (#283)  
>Crash fix [drdump  310428]  
>Add return value to AlphaBltSubPic.patch by Aleksoid1978  
>Fix possible crash in logger  
>Missing change in previous commit.  
>add dark frame to properties dialog - support opening file location directly (#279)* Show dark frame on properties dialog* support opening file location from menus and shortcuts  
>Crash fix [drdump  310802]  
>Crash fix [drdump  310345]  
>avoid tooltip crash when using arrow keys (#271)  
>set CDC font before drawing hotkey edit button (#273)  
>default seekbar height to DEF_MODERN_SEEKBAR_HEIGHT instead of MIN (#280)  
>Crash fix [drdump  310394]  
>Crash fix [drdump 311275]  
>check dialog font before assigning to listctrl (#263)  
- - - - - - - - - - - - - - - - - - - - - - - - - - - 


2020-01  
Contributor(s):  
adipose  
clsid2  
>Patch to improve visual for windowed fullscreen mode (#206)  
>improvements to modern volumectrl behavior (#198)  
>Transifex updates (#221)  
>Fix for certain SSA subs that use opaque boxes.  
>Support rewinding to first volume of RAR archives (#228)  
>Fix: bypass YDL when URL points to a media file  
>Apply DWMWA_CLOAK only if a delay is set  
>improve performance of playlists (#231)  
>enable themed messageboxes on Misc. Options, themed tooltips on subtitles (#224)  
>Colorprofile support for coverart (#238)  
>Reverted changes to transifex script, new translations (#232)  
>Added LumaSharpen and Sepia shaders  
>Applied AStyle code formatting  
>1.9.1  
>Changed logic of "Auto Fit (larger only)". Previously it would resize if the video was too large to fit on the screen. Now it always resizes when the video is larger then the specified auto fit factor percentage of the screen size.  
>Improvements to RFS (#230)  
>Transifex updates and fix for sync script (#217)  
>Fix for setting subpic texture size.  
>Add shortcut for toggling default subtitle style option (#209)  
>fix issues caused by PR 162, cache only after wnd creation (#243)  
>do not override dialog fonts for theme (#201)avoid getclientmetrics scaling issues  
>Update copyright year  
>Transifex updates (#222)  
>do not change colored dark toolbars to monochrome (#207)  
- - - - - - - - - - - - - - - - - - - - - - - - - - - 


2019-12  
Contributor(s):  
adipose  
clsid2  
>Import RARFileSource into code tree  
>bypass DPI adjustment when restoring original window size (#190)  
>Workaround for Intel driver bug. It doesn't actually support the minimum brightness/hue color control values that it claims it supports.  
>1.9.0  
>Correctly set default modern seekbar height.  
>fix memory leaks with mp3 coverart (CMPCPngImage) (#191)  
>Support filtering accel table  
>Support sorting accel table  
>support manual search for subtitles (#176)  
>Improve PGS subtitle positioning (#172)  
>Make modern volume control bar a bit higher  
>Bugfix for recent sub texture change  
>Limit subtitle texture size to the video area size. Previously it could use the size of the entire virtual desktop, which can be huge in case of multiple monitors. This change should improve performance and also reduce GPU memory usage in such multi monitor environments.  
>fix for d3d fullscreen toggle (#188)  
>allow vertical alignment of video to window (#180)  
>fix for d3d fullscreen toggle (#185)  
- - - - - - - - - - - - - - - - - - - - - - - - - - - 


2019-11  
Contributor(s):  
SquallATF  
adipose  
clsid2  
>Set default modern seekbar height to 12  
>Increase size of options dialog to fit wider fonts and long translations (#164)  
>Support filling volume and seek controls instead of showing dragger (#155)  
>Hide filetype icon in statusbar.  
>Reloading a stream with ctrl+e will now call youtube-dl again to get a fresh URL.  
>Fix listctrl measurement bug (#162)  
>MODERNSEEKBAR_HEIGHT rename (#158)  
>fix build with win10 sdk (#154)  
>fix constant name  (#157)  
>allow color toolbars to use normal graying (#166)  
>Fix crash when monitor is detached while playing (#152)  
>Fix for playlist scrolling on opening mpc (#160)  
- - - - - - - - - - - - - - - - - - - - - - - - - - - 


2019-10  
Contributor(s):  
adipose  
clsid2  
>In case of dark theme use "toolbar_dark.bmp/png/svg" as filename for loading custom toolbar.  
>Hardcode the languages supported by OpenSubtitles/SubDB, instead of retrieving it each time the options page is viewed.  
>1.8.8  
>When changing the video renderer, select the internal subtitle renderer if that was incompatible with the previous video renderer.  
>Various fixes to playlist (drag&drop, playing item in dark mode) (#148)* Show bullet next to active item in playlist* Fix colors for highlighted playlist item* Improve drag and drop draw code  
>Added dark theme (#90)Added dark theme. Can be activated through: menu > view > dark themeBig thanks to adipose for developing this theme.  
>Added some sanity checks in PGS parser to avoid crashes on malformed files.Based on patch from MPC-BE.  
>Two dark theme fixes (#142)Fix for tooltip flicker with win7 classic theme.Fix sort issue on External Filters.  
>Set LoopFolderOnPlayNextFile option to disabled as default value  
>Add support for MPC Video Rendererhttps://github.com/Aleksoid1978/VideoRenderer  
>Add dark window frames to all dialogs--deprecate custom CFrameWnd (#146)Improved dark window frames  
- - - - - - - - - - - - - - - - - - - - - - - - - - - 


2019-07  
Contributor(s):  
clsid2  
>1.8.7  
>Remove comment  
>Option for looping folder when playing next/previous file. Enabled by default.  
- - - - - - - - - - - - - - - - - - - - - - - - - - - 


2019-06  
Contributor(s):  
clsid2  
>Fix DVD region code bypass  
- - - - - - - - - - - - - - - - - - - - - - - - - - - 


2019-05  
Contributor(s):  
clsid2  
>Increase threshold for adding files to playlist from 500ms to 1s. This check triggers when the player is executed multiple times in very short period, e.g. due to selecting multiple files in Explorer.  
>When seeking to a previous keyframe while video is playing, jump at least 500ms to avoid getting stuck on a frame. When paused it still goes to nearest keyframe.  
>Add comment about rare hang in filter graph builder  
- - - - - - - - - - - - - - - - - - - - - - - - - - - 


2019-04  
Contributor(s):  
clsid2  
>Some cleanup for build script and project files  
>Also add VP9 profile 2 choice  
>Added advanced option to specify the preferred video format for selecting the desired stream from Youtube-DL results.  
- - - - - - - - - - - - - - - - - - - - - - - - - - - 


2019-03  
Contributor(s):  
clsid2  
>Small tweak for LAV version check  
>Fix commit d40420c289. Add preferred subtitle filter the correct way.  
>Some changes to LAV settings code  
>Load the desired subtitle renderer with high merit. This allow using it even when the filter is registered on the system with value MERIT_DO_NOT_USE.  
>Fix: don't use YDL when an URL points to a file  
>Fix playlist positioning on a non-primary screen with extended Desktop.New code uses virtual desktop size, but doesn't check if the monitor is active.  
>Load XySubFilter also for files without audio  
>Catch exceptions when removing shader cache files.  
>1.8.5  
>1.8.6  
>Change default DVB settings to always rebuild filter graph. This is most reliable setting.  
>Disable global media keys option by default on Windows 10  
- - - - - - - - - - - - - - - - - - - - - - - - - - - 


2019-02  
Contributor(s):  
clsid2  
>CoverArt improvements. Prefer image with same filename. Reduce false positives with heuristic search.  
>Increase max Pan&Scan zoom factor to 5x  
>Fix selection of default osd font on some systems  
>Update LAV settings  
- - - - - - - - - - - - - - - - - - - - - - - - - - - 


2019-01  
Contributor(s):  
clsid2  
>Ignore auto-zoom setting when remember window size is enabled.  
>Update zita-resampler to 1.6.2  
>Fix: ellipsis character was trimmed off from beginning or end of subtitle lines  
- - - - - - - - - - - - - - - - - - - - - - - - - - - 


2018-12  
Contributor(s):  
Caleb Xu  
clsid2  
>Simplify some d3d9 code  
>Command line option for downloading with YDL  
>Increased maximum image size for "Save Thumbnails" to 3840. Max columns is now 16 and max rows is now 40.  
>More YDL improvements  
>Allow YDL to download separate audio and video files when ffmpeg.exe is available for merging them.  
>Fix crash in null renderer. Also support more mediatypes.  
>Shorten tooltip width in Internal Filters (#51)"IDS_INTERNAL_LAVF_WMV" is missing line breakshttps://trac.mpc-hc.org/ticket/5905#comment:4  
>Improve YDL stream selection code  
>1.8.4  
- - - - - - - - - - - - - - - - - - - - - - - - - - - 


2018-11  
Contributor(s):  
clsid2  
>Enable "Save a copy" for streaming from an URL  
>Fix support for UHD Bluray with internal source filter  
>Improve StreamDriveThru code  
>When playing an URL generated by Youtube-DL, the "Save a copy" function now uses Youtube-DL to download the stream. It downloads the highest quality single file version of the stream.  
>Store ydlSourceURL value in playlist file  
>Fix 0d5ddc8  
- - - - - - - - - - - - - - - - - - - - - - - - - - - 


2018-10  
Contributor(s):  
clsid2  
>1.8.3  
>Improve cover art selection when a file contains multiple embedded covers  
>Fix subtitle track selection bug  
- - - - - - - - - - - - - - - - - - - - - - - - - - - 


2018-09  
Contributor(s):  
clsid2  
>Added code to exclude certain URLs from being processed by Youtube-DL  
>Add AV1 video to internal filter list  
>Remove some unused code  
>1.8.2  
>Fix: YDL links were added to recent file list even if keep history was disabled  
>1.8.0  
>Allow using externally installed LAV Filters as internal filters when our own LAV Filters folder doesn't exist.  
>Add WMA to internal filters list  
>Added advanced option for controlling usage of Youtube-DL  
>1.8.1  
>1.7.18  
>Limit max height returned by Youtube-DL to 1440 by default. Higher resolutions may not play smoothly due to current bandwidth restrictions from Youtube for non-browser applications.  
>Correct default value of SaveImageCurrentTime  
>If a playlist entry points to an URL, then display the label in player title bar instead of the URL.  
>Fix crash when accessing info page of web interface.  
>Fix: correct millisecond value in SaveImage filename  
>Added advanced option "AllowInaccurateFastseek", which is enabled by default. When enabled fast seek is allowed to be very inaccurate (max difference 20 seconds). When disabled the allowed inaccuracy is much lower. For example 30% of jump size.This fixes slow seeking in files with huge keyframe intervals (e.g. once per 250 frames).  
- - - - - - - - - - - - - - - - - - - - - - - - - - - 


2018-08  
Contributor(s):  
Nicholas Parkanyi  
clsid2  
>Support for parsing links with youtube-dl  
>Fix hiding of playlist when launching in fullscreen.  
>Tweak max jump inaccuracy  
>Fix for commit 191f60d  
>Limit YDL to max UHD resolution by default, since most systems are unable to play higher resolutions.  
>Changed default subtitle font to Tahoma. This should fix a problem where certain Unicode characters are displayed as squares.  
- - - - - - - - - - - - - - - - - - - - - - - - - - - 


2018-07  
Contributor(s):  
Ayu  
clsid2  
>If seeking to a keyframe (fast seek) is too inaccurate, then a normal seek is now performed instead. Also improved responsiveness when dragging the seekbar.  
>Update ResizableLib to v1.5.1  
>Add multiple monitors support for color management  
>Removed unused MediaInfo project files  
- - - - - - - - - - - - - - - - - - - - - - - - - - - 


2018-06  
Contributor(s):  
clsid2  
>Use millisecond precision in SaveImage filename  
>Change website and update checker URLs  
>Fixed: the "Ignore System Channel Mixer" audio renderer option was not saved  
>1.7.17  
- - - - - - - - - - - - - - - - - - - - - - - - - - - 


2018-05  
Contributor(s):  
clsid2  
>Fix ticket 6416  
>Compile fix for VS15.7  
- - - - - - - - - - - - - - - - - - - - - - - - - - - 


2018-04  
Contributor(s):  
clsid2  
>1.7.16  
>Fix for commit e94a884f35  
>Skip seeks when duration is unknown  
- - - - - - - - - - - - - - - - - - - - - - - - - - - 


2018-03  
Contributor(s):  
clsid2  
>Limit inaccuracy of fast seek to 20 seconds. This also fixes seeking in files that have invalid keyframe timestamps.  
- - - - - - - - - - - - - - - - - - - - - - - - - - - 


2018-02  
Contributor(s):  
Hendrik Leppkes  
clsid2  
>HdmvClipInfo: Support UHD Blu-ray discs (#199)  
>1.7.15  
>Skip seeks if duration is unknown.Previously it would seek to zero position.  
- - - - - - - - - - - - - - - - - - - - - - - - - - - 


2018-01  
Contributor(s):  
clsid2  
>Fix explorer context menu option state display  
>Small tweaks to renderer settings preset  
>Added warning message when enabling "Full Floating Point Processing" renderer setting.Also changed its settings name to reset it to the default value when updating from old version.  
>Render subtitles relative to the window by default.ASS/SSA are still rendered relative to video by default, because those sometimes need accurate positioning.This behavior can be adjusted in the options. This patch just changes the default value of the setting.  
>Added command line switches for accessing settings of internal LAV Filters./configlavsplitter /configlavaudio /configlavvideo  
>Added new settings from LAV Filters  
>Disable DrDump Crash Reporter  
>Use external MediaInfo DLL  
>Use DX binaries  
>1.7.14  
>Small edit list editor improvement  
- - - - - - - - - - - - - - - - - - - - - - - - - - - 


2017-12  
Contributor(s):  
clsid2  
>Simplify code for settings renderer merits  
- - - - - - - - - - - - - - - - - - - - - - - - - - - 


2017-10  
Contributor(s):  
clsid2  
>Fix /open and /close command line handling.Fixes ticket 6303  
- - - - - - - - - - - - - - - - - - - - - - - - - - - 


2017-09  
Contributor(s):  
clsid2  
>Fix ticket 6308  
>Extra logging for OpenSubtitles code  
>Fix possible null pointer dereference  
- - - - - - - - - - - - - - - - - - - - - - - - - - - 


2017-08  
Contributor(s):  
clsid2  
>Added advanced options for tweaking filename of SaveImage function. This allows excluding timestamp and position.ToDo: translations  
- - - - - - - - - - - - - - - - - - - - - - - - - - - 


2017-07  
Contributor(s):  
clsid2  
>Small fix  
- - - - - - - - - - - - - - - - - - - - - - - - - - - 

