------------------------------------------------------------------------------
 NINJA Patcher *BETA*                                             version 2.0
 Written by Derrick Sobodash                                   Copyright 2006
 Released on July 26, 2006                     http://www.cinnamonpirate.com/
------------------------------------------------------------------------------

 This is a beta version of what will become NINJA 2.0. It currently supports
 all expected functions that will be in the final version. I am releasing
 this to get help with rigorous bug testing.

 If you are willing to submit a localization, please translate the right
 column of localize.txt to your native language and mail it to me at
 suderui (a t) sina (d o t) com

------------------------------------------------------------------------------

 INSTALL

 Extract this archive to any folder you would like to install it to and
 run ninja.exe. You will be prompted to create file associations. NINJA can
 associate itself with RUP, IPS, PPF, FFP, PAT and GDIFF patch files.

 If you change your mind about associating files with NINJA, click
 About->Associate and uncheck the option to allow NINJA to manage your file
 associations. This will not restore your former associations, but it will
 prevent NINJA from taking over those associations at each load.

 To create patches, run creator.exe.

------------------------------------------------------------------------------

 FEATURES

 * Capable of creating and applying patches between both directories and
   single files

 * XOR patches allows a NINJA patch to both patch a file and restore its
   original form

 * Support for International Patching Standard (IPS) with the LunarCut
   format extension.

 * Support for PlayStation Patch Format (PPF) 1.0, 2.0 and 3.0

 * Support for Generic Diff Format (GDIFF) binary patches

 * Support for FireFlower Patcher (FFP/PAT) textual patches. Can use as
   both a patcher an unpatcher

 * Support for Microsoft File Compare output; can use as both a patcher
   and unpatcher (Example: fc /b FILE1 FILE2 >output.pat)

 * Support for applying xdelta Format patches if you have an xdelta patcher
   present in the NINJA folder (xdelta.exe). You can find one at
   Evan Jones's webpage (http://evanjones.ca/software/xdelta-win32.html).

 * Double-click file application with associations for RUP, IPS, PPF, PAT,
   FFP and GDIFF

 * File icons to match default Windows XP Luna, Windows Vista or CrystalXP
   "Crystal Clear" BricoPack

 * PHP command line back-end that can be used on any PHP-capable OS
   (RISC OS, Mac OSX, Linux, FreeBSD, QNX RTP, SGI IRIX, OS/2 and Novell
   NetWare); back-end supports all NINJA features except the GUI

 * Format supports addressing in files up to 32,317,006*10^600 bytes;
   sadly, this implementation only supports up to 9,223,372,036,854,775,807.

 * NINJA patches support a struct of internal patch info so you can store
   the author, version number, game title, game genre, patch language,
   release date, patch homepage and a 1074 character summary

 * Drag-and-drop in file names to fields for quicker patch creation

 * Nintendo 8-bit: Supports patching around NES headers; support for all
   NES formats except Pasofami (interNES, Famtasia and UNIF); one patch
   will work on NES, FAM and UNF ROMs

 * Super Nintendo: Supports patching around SNES headers; deinterleaving of
   Game Doctor SF3, Professor SF2, Super UFO and Multi Game Doctor formats;
   can preserve NSRT headers; one patch will work on SMC, SWC, FIG, 048, 
   058, 078, SFC, MGD, UFO, GD3 and GD7 ROMs

 * Nintendo 64: Supports deinterleaving of Doctor V64 ROMS; one patch will
   work on both Z64 and V64 ROMs

 * Game Boy: Supports removal of SmartCart Game Boy headers

 * Sega Master System: Supports deinterleaving of Super Magic Drive ROMs;
   one patch will work on both SMS and SMD ROMs

 * Sega Genesis: Supports deinterleaving of Super Magic Drive ROMs; one
   patch will work on both BIN and SMD ROMs

 * PC-Engine: Supports removal of Magic Super Griffin headers

 * Atari Lynx: Supports patching around Atari Lynx headers; one patch will
   work on LNX/RAW and LYX ROMs

------------------------------------------------------------------------------

 TO DO

 * Improve information in format.html for clarity

------------------------------------------------------------------------------

 REQUIREMENTS

 * For the GUI: a Windows operating system with support for
   Microsoft .NET Framework 2.0 Redistributable Package (available from
   Windows Update or http://msdn.microsoft.com/netframework/)

 * For the command prompt: access to a command line and PHP installed

------------------------------------------------------------------------------

 CHANGELOG

 July 26 Beta - Added a check to make sure target file is writeable before
   patching. Added icon previews to the GUI. Added a detection for GB ROMs
   using magic bytes provided by Aerdan. Rewrote a lot of NINJA code to
   use escape strings.

 July 6 Beta - Added support for using NINJA as a frontend for applying
   xdelta format patches. Required you have the xdelta executable
   (xdelta.exe) in the program path. Added icons for xdelta format and
   options for it to the file association menu. Fixed a glitch that was
   preventing users' language selection from actually being applied. Edited
   the German translation to match Tauwasser's suggestions.

 July 5 Beta - Retored ninja.bat for command-line users who refuse to
   install .NET 2.0. Fixed a glitch that prevented the command line options
   from showing when you entered no arguments.

 July 4 Beta - Added a "Please Wait" dialog to the patch creator since it
   can take a while, and otherwise people may think the application has
   frozen (seems a little flaky because all the power goes to the console
   read). Fixed IPS patching to work with the corrupt patches some IPS
   creators produce that do not end in "EOF".

 July 3 Beta - Changed the About dialog's address to the new page at
   http://ninja.cinnamonpirate.com/.

 June 29 Beta 3 - Fixed a critical patch creation error that prevented users
   from being able to create directory patches. I have no idea how this
   managed to escape under the radar for so long. Got rid of the pop-up
   window when creating patches.

 June 29 Beta 2 - Redesigned the registry entry map to group all NINJA
   file associations together. Fixed a glitch that was causing the PPF
   Crystal icon to display as the GDIFF icon.

 June 29 Beta - Fixed a code glitch that blocked GDIFF from even being
   useable. Fixed a code glitch that was preventing PPF 1.0 and PPF 3.0
   files from being applied. Added support for FireFlower Patch format
   files, a format created by Kuma (kuma_yeb@d1.dion.ne.jp) in 2000 for
   distributing NeoGeo graphic and color hacks. Added support for using
   Microsoft File Compare binary comparison output as a file patch
   (example: fc /b FILE1 FILE2 >output.pat). Added icons for GDIFF and
   FireFlower Patch formats. Moved icons to an external icons.dll library.

 June 28 Beta - Added Brazillian Portugese localization (dormento). Added
   support for Generic Diff Format (GDIFF) based on specs submitted to the
   W3C August 21, 1997 (untested). Fixed a UI glitch where clicking the "X"
   button to close a form instead of the close button left the main
   patching window locked.

 June 26 Beta - Updated the patcher frontend so you no longer need to
   restart to switch languages. Added Italian localization (Feitan).

 June 24 Beta - Included icon settings into the program itself so NINJA no
   longer needs the external *.bat file. Cleaned up the output to the
   messgae window. Added localization pages for German (KaioShin), Chinese
   Simplified (Jacky Waiss), Traditional Chinese (Yu-Chen Shih), Russian
   (Misha Golub) and Spanish (Kitsune Sniper).

 June 20 Beta - Change to the file format to add more length for patch info
   and more longer file names. Updated patch creator to allow creation of
   UTF-8 or system codepage based patches. Users running NINJA from the
   Windows Command Line are advised to edit ninja.php and select their
   system's codepage (default is ASCII).

 June 14 Beta - Fixed SNES deinterleaving support, added SNES 64mbit
   deinterleaving, fixed SMS deinterleaving, added N64 deinterleaving,
   added support for iNES/FAM/UNIF formats, added NSRT header preservation,
   added Lynx header preservation

------------------------------------------------------------------------------

 COMMAND-LINE USEAGE

 NINJA does two things: makes patches and applies them. This version can
 apply NINJA 2.0, PPF, IPS, FFP, PAT and GDIFF patches. In the case of PPF,
 PAT, FFP and NINJA, the internal patch info will be displayed in the
 console for you to read.

 The command lines are:

   * Create - ninja2.bat SOURCE MODIFIED FILE TYPE INFO
                            |       |     |    |    |
                            |       |     |    |    +- Text info file
                            |       |     |    +------ Format of data
                            |       |     +----------- File to create
                            |       +----------------- Modified file/folder
                            +------------------------- Original file/folder
   * Apply  - ninja2.bat PATCH TARGET
                           |     |
                           |     +-------------------- File/Folder to patch
                           +-------------------------- Patch file

 The text info file must be a plain text file of 8 lines. The data expected
 on each line is:

   1: Author      (  84 characters max)
   2: Version     (  11 characters max)
   3: Title       ( 256 characters max)
   4: Genre       (  48 characters max)
   5: Language    (  48 characters max)
   6: Date        (   8 characters max) (YYYYMMDD)
   7: Website     ( 512 characters max)
   8: Description (1074 characters max)

 Data exceeding the limit for each line will be ignored. If no file is
 specified, blank bytes will be entered for the file info.

 Allowed data types are as follows:

   raw - Raw Binary (0)               gb  - Nintendo Game Boy (5)
   nes - Nintendo/Famicom (1)         sms - Sega Master System/Game Gear (6)
   fds - Famicom Disk System (2)      mega- Sega Genesis/Megadrive (7)
   snes- Super Nintendo/Famicom (3)   pce - NEC TurboGrafx-16/PC-Engine (8)
   n64 - Nintendo 64 (4)              lynx- Atari Lynx (9)

 If no data type is specified, raw is assumed.

 If your original data is a folder, the modified data must be a folder with
 the same structure. To decrease patch creation time, it is recommended
 you include only modified files in the modified folder, otherwise NINJA
 will be forced to scan every single file.

 NINJA will fail if you attempt to create a patch between a file
 and a folder.

------------------------------------------------------------------------------

 FILE FORMAT

 NINJA 2.0 follows a much simpler file format than its predecessor. Each
 patch consists of 3 regions: HEADER, INFO and DATA. HEADER and INFO make
 up the first sector of the patch (1024 bytes).

 @HEADER@
 {
   NINJA_MAGIC - string "NINJA" (5 bytes)
   NINJA_VER   - char "2"       (1 byte )
 }

 @INFO@
 {
   PATCH_ENC   - int ENC      (   1 byte ) // Info text encoding
                                           // 0: System codepage / 1: UTF-8
   PATCH_AUTH  - string AUTH  (  84 bytes) // Author
   PATCH_VER   - string VER   (  11 bytes) // Version
   PATCH_TITLE - string TITLE ( 256 bytes) // Title
   PATCH_GENRE - string GENRE (  48 bytes) // Genre
   PATCH_LANG  - string LANG  (  48 bytes) // Language
   PATCH_DATE  - string DATE  (   8 bytes) // Date as YYYYMMDD
   PATCH_WEB   - string WEB   ( 512 bytes) // Website
   PATCH_DESC  - string DESC  (1074 bytes) // Info (New line marked by "\n")
 }

 @DATA@
 {
   COMMAND     - int COMMAND  (  1 byte )

   if COMMAND == 0x1     // Open File|Close Current
   {
     FILE_N_MUL    - int N_MUL     (        1 byte ) // 0 Signals single-file
     FILE_N_LEN    - int N_LEN     (    N_MUL bytes) // Length of file name
     FILE_NAME     - string NAME   (    N_LEN bytes) // File name
     FILE_TYPE     - int TYPE      (        1 byte ) // File format
     FILE_SSIZE_MUL- int SSIZE_MUL (        1 byte )
     FILE_SSIZE    - int SIZE      (SSIZE_MUL bytes) // Source file size
     FILE_MSIZE_MUL- int MSIZE_MUL (        1 byte )
     FILE_MSIZE    - int MIZE      (MSIZE_MUL bytes) // Modified file size
     FILE_SMD5     - string SMD5   (       16 bytes) // Source MD5sum
     FILE_MMD5     - string MMD5   (       16 bytes) // Modified MD5sum

     if SSIZE > MSIZE
     {
       FILE_MAGIC   - char "M"        (       1 byte ) // Source overflow
       FILE_OVER_MUL- int OVER_MUL    (       1 byte )
       FILE_OVER    - int OVER        (OVER_MUL bytes) // Overflow length
       FILE_OVERFLOW- string OVERFLOW (    OVER bytes) // Lost from modified
     }

     else if MSIZE > SSIZE
     {
       FILE_MAGIC   - char "A"        (       1 byte ) // Modified overflow
       FILE_OVER_MUL- int OVER_MUL    (       1 byte )
       FILE_OVER    - int OVER        (OVER_MUL bytes) // Overflow length
       FILE_OVERFLOW- string OVERFLOW (    OVER bytes) // Gained in modified
     }
   }

   else if COMMAND == 02 // XOR Patch
   {
     PATCH_OFF_MUL- int OFF_MUL (      1 byte )
     PATCH_OFF    - int OFF     (OFF_MUL bytes) // Patch offset
     PATCH_LEN_MUL- int LEN_MUL (      1 byte )
     PATCH_LEN    - int LEN     (LEN_MUL bytes) // Patch length
     PATCH_XOR    - string XOR  (    LEN bytes) // XOR string
   }

   else if COMMAND == 0x0
     // Terminate patch
 }

------------------------------------------------------------------------------
