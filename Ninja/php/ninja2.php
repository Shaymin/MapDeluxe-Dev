#!/usr/bin/php -q
<?php

/*
    NINJA - PHP Edition
    Copyright (C) 2006 Derrick Sobodash
    Version: 2.0
    Web    : http://www.cinnamonpirate.com/
    E-mail : d-xiansheng at sourceforge dot net

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program (license.txt); if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/******************************************************************************
**
** CONFIGURE
**
******************************************************************************/

// System Codepage:
//
// This is for conversion of UTF-8 text. The Windows Console has problems
// displaying UTF-8 input, so we have to convert the stored UTF-8 to another
// console-friendly codepage.
//
// Linux and FreeBSD users may comment this out, since those operating
// systems do not have issues with UTF-8 on the console. Windows users are
// advised to set this to the language of their machines.
//
// Some common language encodings are commented out below:

// RAW ASCII
define("CODEPAGE", "ASCII");

// Japanese Shift-JIS
//define("CODEPAGE", "SJIS");

// Japanese EUC
//define("CODEPAGE", "EUC");

// Chinese GuoBiao2312
//define("CODEPAGE", "CN-GB");

// Chinese EUC
//define("CODEPAGE", "EUC-CN");

// Chinese BIG-5
//define("CODEPAGE", "BIG-5");

// Korean EUC
//define("CODEPAGE", "EUC-KR");

// Hungarian/Serbian/Bosnian/Croatian/Czech/Polish/Romanian
//define("CODEPAGE", "ISO_8859-2");

// Russian (Cyrillic text)
//define("CODEPAGE", "ISO_8859-5");

// Arabic
//define("CODEPAGE", "ISO_8859-6");

// Greek
//define("CODEPAGE", "ISO_8859-7");

// Hebrew
//define("CODEPAGE", "ISO_8859-8");

// Turkish
//define("CODEPAGE", "ISO_8859-9");


/******************************************************************************
**
** SOURCE CODE BEGINS
**
******************************************************************************/

// Load GMP if it is present on the host system
// GMP is needed for safe, 64-bit math
if(!extension_loaded('gmp'))
{
      
  if (strtoupper(substr(PHP_OS, 0, 3)) === 'WIN')
    dl('php_gmp.dll');
  else
    dl('gmp.so');

}
else
  die("Fatal Error: GMP library was not found\r\n\r\n");

// Load mbstring if it is present on the host system
// mbstring is needed for UTF-8
if(!extension_loaded('mbstring'))
{
      
  if (strtoupper(substr(PHP_OS, 0, 3)) === 'WIN')
    dl('php_mbstring.dll');
  else
    dl('mbstring.so');

}
else
  die("Fatal Error: mbstring library was not found\r\n\r\n");



/******************************************************************************
**
** DEFINITIONS
**
******************************************************************************/

// Bits
define("KBIT", 0x80);
define("MBIT", 0x20000);
define("GBIT", 0x8000000);

// Bytes
define("KBYTE", 0x400);
define("MBYTE", 0x100000);
define("GBYTE", 0x40000000);

// ROM Headers
define("SNES_HEADER", 0x200);


/******************************************************************************
** create_ninja2()
*******************************************************************************
** syntax: void create_ninja2(string source, string modified, string target[,
	        string type, array patch_info])
*******************************************************************************
** Creates a NINJA 2.0 format patch.
******************************************************************************/

function create_ninja2($source, $modified, $target, $type = null, $patch_info = null)
{

  $fo = fopen($target, "wb");

  // Write the ID tag
  fputs($fo, "NINJA2");

  // Build the patch info block
  if(!$patch_info)
    fputs($fo, str_pad("", 2042, "\0"));
  else
  {

    echo("Writing patch info block ...\r\n");

    // Encoding
    fputs($fo, chr($patch_info["enc"]));

    // Author
    fputs($fo, str_pad($patch_info["auth"], 84, "\0"));

    // Version
    fputs($fo, str_pad($patch_info["ver"], 11, "\0"));

    // Title
    fputs($fo, str_pad($patch_info["title"], 256, "\0"));

    // Genre
    fputs($fo, str_pad($patch_info["genre"], 48, "\0"));

    // Language
    fputs($fo, str_pad($patch_info["lang"], 48, "\0"));

    // Year
    fputs($fo, str_pad($patch_info["year"], 8, "\0"));

    // Website
    fputs($fo, str_pad($patch_info["web"], 512, "\0"));

    // Description
    fputs($fo, str_pad($patch_info["desc"], 1074, "\0"));

  }

  // If the source and modified fields are directories, read modified
  // and make patches of the files in modified against source.
  if(is_dir($source) && is_dir($modified))
  {

    echo("Creating patch between directories ...\r\n");
    $files = list_dir($modified, "*.*", "/", TRUE);

    // Clean out the modified path from the files list
    for($i = 0; $i < count($files); $i ++)
      $files[$i] = substr($files[$i], strlen($modified));

    for($i = 0; $i < count($files); $i ++)
    {

      echo("Generating ".$files[$i]." ...\r\n");
      fputs($fo, ninja_block($source, $modified, $type, $files[$i]));

    }

    fputs($fo, "\0");

  }

  // Otherwise do a single file patch.
  else if(is_file($source) && is_file($modified))
  {

    echo("Creating patch between files ...\r\n");

    fputs($fo, ninja_block($source, $modified, $type) . "\0");

  }

  else
    die("Error: create_ninja2(): Source and modified are not the same type\r\n\r\n");

  fclose($fo);

  if(filesize($target) == 2 * KBYTE + 1)
  {

    echo("Files have no differences. Deleting patch ...\r\n");
    unlink($target);

  }

  echo("Complete!\n");

}


/******************************************************************************
** ninja_block()
*******************************************************************************
** syntax: void ninja_block(string source, string modified, string type)
*******************************************************************************
** Builds a NINJA 2.0 single file patch between two files and returns the
** resulting patch block as a string.
******************************************************************************/

function ninja_block($source, $modified, $type, $file = null)
{

  $patch = "\x01";

  if($file == null) $file = " ";
  $file2 = substr($file, 1);

  // File Name
  $namelen = rtrim(ppack(strlen($file2), 64), "\0");

  if(strlen($namelen) > 0)
    $patch .= rtrim(ppack(strlen($namelen), 64), "\0") . $namelen . $file2;
  else
    $patch .= "\0";

  // File Type
  $patch .= chr($type);

  // Perform conversion if needed depending on file type
  switch($type)
  {

    // Nintendo Entertainment System/Famicom
    case "1":
      echo("Reading as Nintendo Entertainment System/Famicom ROM ...\r\n");
      $fo = fopen("ninja.src", "wb");
      list($rom, $header) = nes_read($source . $file);
      fputs($fo, $rom);
      fclose($fo);
      $fo = fopen("ninja.mod", "wb");
      list($rom, $header) = nes_read($modified . $file);
      fputs($fo, $rom);
      fclose($fo);
      $source = "ninja.src";
      $modified = "ninja.mod";
      $file = "";
      unset($rom, $header);
      break;

    // Super Famicom
    case "3":
      echo("Reading as Super Famicom ROM ...\r\n");
      $fo = fopen("ninja.src", "wb");
      list($rom, $header) = sfam_read($source . $file);
      fputs($fo, $rom);
      fclose($fo);
      $fo = fopen("ninja.mod", "wb");
      list($rom, $header) = sfam_read($modified . $file);
      fputs($fo, $rom);
      fclose($fo);
      $source = "ninja.src";
      $modified = "ninja.mod";
      $file = "";
      break;

    // Nintendo 64
    case "4":
      echo("Reading as Nintendo 64 ROM ...\r\n");
      $fo = fopen("ninja.src", "wb");
      fputs($fo, n64_read($source . $file));
      fclose($fo);
      $fo = fopen("ninja.mod", "wb");
      fputs($fo, n64_read($modified . $file));
      fclose($fo);
      $source = "ninja.src";
      $modified = "ninja.mod";
      $file = "";
      break;

    // Game Boy
    case "5":
      echo("Reading as Game Boy ROM ...\r\n");
      $fo = fopen("ninja.src", "wb");
      fputs($fo, gb_read($source . $file));
      fclose($fo);
      $fo = fopen("ninja.mod", "wb");
      fputs($fo, gb_read($modified . $file));
      fclose($fo);
      $source = "ninja.src";
      $modified = "ninja.mod";
      $file = "";
      break;

    // Sega Master System
    case "6":
      echo("Reading as Sega Master System ROM ...\r\n");
      $fo = fopen("ninja.src", "wb");
      fputs($fo, sms_read($source . $file));
      fclose($fo);
      $fo = fopen("ninja.mod", "wb");
      fputs($fo, sms_read($modified . $file));
      fclose($fo);
      $source = "ninja.src";
      $modified = "ninja.mod";
      $file = "";
      break;

    // Sega Megadrive
    case "7":
      echo("Reading as Sega Megadrive ROM ...\r\n");
      $fo = fopen("ninja.src", "wb");
      fputs($fo, mega_read($source . $file));
      fclose($fo);
      $fo = fopen("ninja.mod", "wb");
      fputs($fo, mega_read($modified . $file));
      fclose($fo);
      $source = "ninja.src";
      $modified = "ninja.mod";
      $file = "";
      break;

    // NEC PC-Engine
    case "8":
      echo("Reading as NEC PC-Engine ROM ...\r\n");
      $fo = fopen("ninja.src", "wb");
      $rom = pce_read($source . $file);
      fputs($fo, $rom);
      fclose($fo);
      $fo = fopen("ninja.mod", "wb");
      $rom = pce_read($modified . $file);
      fputs($fo, $rom);
      fclose($fo);
      $source = "ninja.src";
      $modified = "ninja.mod";
      $file = "";
      break;

    // Atari Lynx
    case "9":
      echo("Reading as Atari Lynx ROM ...\r\n");
      $fo = fopen("ninja.src", "wb");
      list($rom, $header) = lynx_read($source . $file);
      fputs($fo, $rom);
      fclose($fo);
      $fo = fopen("ninja.mod", "wb");
      list($rom, $header) = lynx_read($modified . $file);
      fputs($fo, $rom);
      fclose($fo);
      $source = "ninja.src";
      $modified = "ninja.mod";
      $file = "";
      unset($rom, $header);
      break;

  }

  // Source File Size
  $temp = rtrim(ppack(filesize($source . $file), 64), "\0");
  $patch .= chr(strlen($temp)) . $temp;

  // Modified File Size
  $temp = rtrim(ppack(filesize($modified . $file), 64), "\0");
  $patch .= chr(strlen($temp)) . $temp;

  // Source MD5
  $patch .= md5_file($source . $file, TRUE);

  // Modified MD5
  $patch .= md5_file($modified . $file, TRUE);

  // Overflow when source file is larger than modified
  if(filesize($source . $file) > filesize($modified . $file))
  {

    // Read in the overflow area
    $fd = fopen($source . $file, "rb");
    fseek($fd, filesize($modified . $file), SEEK_SET);
    $overflow = fread($fd, filesize($source . $file) - filesize($modified . $file));
    fclose($fd);

    // Write the overflow
    $len = rtrim(ppack(strlen($overflow), 64), "\0");
    $patch .= "M" . chr(strlen($len)) . $len . pxor($overflow, "\xff");

    unset($fd, $overflow, $len);

  }

  // Overflow when modified file is larger than source
  if(filesize($source . $file) < filesize($modified . $file))
  {

    // Read in the overflow area
    $fd = fopen($modified . $file, "rb");
    fseek($fd, filesize($source . $file), SEEK_SET);
    $overflow = fread($fd, filesize($modified . $file) - filesize($source . $file));
    fclose($fd);

    // Write the overflow
    $len = rtrim(ppack(strlen($overflow), 64), "\0");
    $patch .= "A" . chr(strlen($len)) . $len . pxor($overflow, "\xff");

    unset($fd, $overflow, $len);

  }

  $verify_changes = $patch;

  // Open the files for comparing
  $src = fopen($source . $file, "rb");
  $mod = fopen($modified . $file, "rb");

  $end_at = filesize($source . $file);

  if(filesize($modified . $file) < filesize($source . $file))
    $end_at = filesize($modified . $file);

  while(ftell($mod) < $end_at && ftell($src) < $end_at)
  {

    if(ftell($mod)+KBYTE < $end_at && ftell($src)+KBYTE < $end_at)
    {

      $test1 = fread($src, KBYTE);
      $test2 = fread($mod, KBYTE);

      if($test1 == $test2)
        continue;

      else
      {

        fseek($src, ftell($src)-KBYTE, SEEK_SET);
        fseek($mod, ftell($mod)-KBYTE, SEEK_SET);

      }

    }

    $src_char = fgetc($src);
    $mod_char = fgetc($mod);
    $src_byte = "";
    $mod_byte = "";

    if($src_char != $mod_char)
    {

      // Write the current offset
      $patchoff = rtrim(ppack(ftell($mod) - 1, 64), "\0");
      if(strlen($patchoff) == 0) $patchoff = "\0";
      $patch .= "\x02" . chr(strlen($patchoff)) . $patchoff;
      unset($patchoff);

      if(ftell($mod) != $end_at && ftell($src) != $end_at)
      {
	      
        while($src_char != $mod_char && ftell($mod) <= $end_at && ftell($src) <= $end_at)
        {
 
          $src_byte .= $src_char;
          $mod_byte .= $mod_char;
          $src_char = fgetc($src);
          $mod_char = fgetc($mod);

	}
	
      }
      else {
          $src_byte .= $src_char;
	  $mod_byte .= $mod_char;
      }

      $patchlen = rtrim(ppack(strlen($mod_byte), 64), "\0");
      $patch .= chr(strlen($patchlen)) . $patchlen . pxor($src_byte, $mod_byte);

    }

  }

  fclose($src);
  fclose($mod);

  // If ninja.src exists, we're working on a temp file. Overwrite the original.
  if(file_exists("ninja.src"))
  {

    unlink($source . $file);
    unlink($modified . $file);

  }

  // Make sure the file actually has changes before returning a patch
  if($verify_changes != $patch)
    return($patch);
  else
  {

    echo("No differences! File will be ignored.\r\n");
    return("");

  }

}


/******************************************************************************
** apply_ninja2()
*******************************************************************************
** syntax: void apply_ninja2(string rup_file, string target)
*******************************************************************************
** Applies a NINJA 2.0 patch. Target must be a directory or file depending
** on what the patch requires.
******************************************************************************/

function apply_ninja2($rup_file, $target)
{

  // Make sure the input is all valid
  if(!file_exists($rup_file))
    die("Error: apply_ninja2(): Unable to open $rup_file\r\n\r\n");
  if(!file_exists($target))
    die("Error: apply_ninja2(): Unable to open $target\r\n\r\n");

  // Open the patch file as a stream
  $rup = fopen($rup_file, "rb");

  // Check the ID Tag
  if(fread($rup, 6) != "NINJA2")
    die("Error: apply_ninja2(): Supplied RUP file does not appear to be a RUP format patch\r\n\r\n");

  echo("Type NINJA 2.0 patch detected!\r\n");
  
  // Skip internal info this time

  fseek($rup, 2048, SEEK_SET);

  // Action Jackson
  $control = fread($rup, 1);

  while($control != "\0")
  {

    // Open file for patching
    if($control == "\x01")
    {

      // Close out current files
      if($fo)
      {

        // Append the original source data if reverting a shrunken modified file      
        if($fileprop["overflow"])
        {

          if($fileprop["ssize"] > $fileprop["msize"] && $fileprop["revert"])
            fseek($fo, $fileprop["msize"], SEEK_SET);
          else
            fseek($fo, $fileprop["ssize"], SEEK_SET && !$fileprop["revert"]);
          fputs($fo, $fileprop["overflow"]);

        }

	// Truncate the file if creating a shrunken modified file
	if($fileprop["ssize"] > $fileprop["msize"] && !$fileprop["revert"])
          ftruncate($fo, $fileprop["msize"]);
        else if($fileprop["ssize"] < $fileprop["msize"] && $fileprop["revert"])
          ftruncate($fo, $fileprop["ssize"]);

        // Restore the header if necessary
        if($header)
        {

          if(file_exists("ninja.src"))
            $tfsize = filesize("ninja.src");
          else
            $tfsize = filesize($target . $fileprop["name"]);
  
          fseek($fo, 0, SEEK_SET);
          $temp = fread($fo, $tfsize);
          fseek($fo, 0, SEEK_SET);
          fputs($fo, $header . $temp);

          unset($temp, $header);

        }

	fclose($fo);

        // If ninja.src exists, we're working on a temp file. Overwrite the original.
        if(file_exists("ninja.src"))
        {

          unlink($target . $fileprop["name"]);
          rename("ninja.src", $target . $fileprop["name"]);

        }

        // Tank our variables
        unset($fileprop, $fo);

      }

      // Read file properties
      $fileprop = array();
      $temp = ord(fgetc($rup));

      if($temp > 0)
      {

        $temp = punpack(fread($rup, $temp));
        $fileprop["name"]  = trim(fread($rup, $temp));

      }

      else
        $fileprop["name"]  = "";

      //$fileprop["name"]  = trim(fread($rup, 256));
      if(strlen($fileprop["name"]) > 0) $fileprop["name"] = "/" . $fileprop["name"];
      $fileprop["type"]  = ord(fgetc($rup));
      $temp = ord(fgetc($rup));
      $fileprop["ssize"] = punpack(fread($rup, $temp));
      $temp = ord(fgetc($rup));
      $fileprop["msize"] = punpack(fread($rup, $temp));
      $fileprop["smd5"]  = bin2hex(fread($rup, 16));
      $fileprop["mmd5"]  = bin2hex(fread($rup, 16));

      echo("Patching $target".$fileprop["name"]."\r\n");

      // Perform conversion if needed depending on file type
      switch($fileprop["type"])
      {

        // Nintendo Entertainment System/Famicom
        case "1":
          echo("Reading as Nintendo Entertainment System/Famicom ROM ...\r\n");
	  $fo = fopen("ninja.src", "wb");
	  list($rom, $header) = nes_read($target . $fileprop["name"]);
          fputs($fo, $rom);
          fclose($fo);
          break;

        // Super Famicom
        case "3":
          echo("Reading as Super Famicom ROM ...\r\n");
          $fo = fopen("ninja.src", "wb");
	  list($rom, $header) = sfam_read($target . $fileprop["name"]);
          fputs($fo, $rom);
          fclose($fo);
          break;

        // Nintendo 64
        case "4":
          echo("Reading as Nintendo 64 ROM ...\r\n");
          $fo = fopen("ninja.src", "wb");
          fputs($fo, n64_read($target . $fileprop["name"]));
          fclose($fo);
	  break;

        // Game Boy
        case "5":
          echo("Reading as Game Boy ROM ...\r\n");
          $fo = fopen("ninja.src", "wb");
          fputs($fo, gb_read($target . $fileprop["name"]));
          fclose($fo);
          break;

        // Sega Master System
        case "6":
          echo("Reading as Sega Master System ROM ...\r\n");
          $fo = fopen("ninja.src", "wb");
          fputs($fo, sms_read($target . $fileprop["name"]));
          fclose($fo);
          break;

        // Sega Megadrive
        case "7":
          echo("Reading as Sega Megadrive ROM ...\r\n");
          $fo = fopen("ninja.src", "wb");
          fputs($fo, mega_read($target . $fileprop["name"]));
          fclose($fo);
          break;

        // NEC PC-Engine
        case "8":
          echo("Reading as NEC PC-Engine ROM ...\r\n");
          $fo = fopen("ninja.src", "wb");
	  $rom = pce_read($target . $fileprop["name"]);
          fputs($fo, $rom);
          fclose($fo);
          break;

        // Atari Lynx
        case "9":
          echo("Reading as Atari Lynx ROM ...\r\n");
	  $fo = fopen("ninja.src", "wb");
	  list($rom, $header) = lynx_read($target . $fileprop["name"]);
          fputs($fo, $rom);
          fclose($fo);
          break;

      }

      if(file_exists("ninja.src"))
        $targetmd5 = md5_file("ninja.src");
      else
        $targetmd5 = md5_file($target . $fileprop["name"]);

      // Check whether we are patching or reverting
      if($targetmd5 == $fileprop["smd5"])
        $fileprop["revert"] = FALSE;

      else if($targetmd5 == $fileprop["mmd5"])
      {

        $fileprop["revert"] = TRUE;
        echo("Reverting to pre-patched file ...\r\n");

      }

      else
        die("Error: apply_ninja2(): File does not match required MD5!\r\n\r\nExpected:\r\n  Source MD5  : ".$fileprop["smd5"]."\r\n  Modified MD5: ".$fileprop["mmd5"]."\r\nFound:\r\n  Target MD5  : ".$targetmd5."\r\n\r\n");

      if(file_exists("ninja.src"))
        $fo = fopen("ninja.src", "r+b");
      else
        $fo = fopen($target . $fileprop["name"], "r+b");

    }

    else if($control == "M")
    {

      // Read the source overflow to a our file properties
      $temp = ord(fgetc($rup));
      $fileprop["overflow"] = pxor(fread($rup, punpack(fread($rup, $temp))), "\xff");

    }

    else if($control == "A")
    {

      // Append end of modified file
      $temp = ord(fgetc($rup));
      $fileprop["overflow"] = pxor(fread($rup, punpack(fread($rup, $temp))), "\xff");

    }

    // Apply XOR patch to current file
    else if($control == "\x02")
    {

      // Get the offset and seek it
      $temp = ord(fgetc($rup));
      $offset = punpack(fread($rup, $temp));
      fseek($fo, $offset, SEEK_SET);

      // Get the patch length
      $temp = ord(fgetc($rup));
      $patchlen = punpack(fread($rup, $temp));

      // Get the patch
      $thispatch   = fread($rup, $patchlen);
      $sourcebytes = fread($fo, $patchlen);
      $thispatch   = pxor($sourcebytes, $thispatch);

      // Insert the patched bytes
      fseek($fo, $offset, SEEK_SET);
      fputs($fo, $thispatch);

      unset($thispatch, $sourcebytes, $patchlen, $offset);

    }

    else
      die("Error: apply_ninja2(): Unknown fatal error (".ord($control)."), file may be corrupt\r\n");

    $control = fgetc($rup);

    //echo ord($control);

  }

  // Append the original source data if reverting a shrunken modified file      
  if($fileprop["overflow"])
  {

    if($fileprop["ssize"] > $fileprop["msize"] && $fileprop["revert"])
      fseek($fo, $fileprop["msize"], SEEK_SET);
    else
      fseek($fo, $fileprop["ssize"], SEEK_SET && !$fileprop["revert"]);
    fputs($fo, $fileprop["overflow"]);

  }

  // Truncate the file if creating a shrunken modified file
  if($fileprop["ssize"] > $fileprop["msize"] && !$fileprop["revert"])
    ftruncate($fo, $fileprop["msize"]);
  else if($fileprop["ssize"] < $fileprop["msize"] && $fileprop["revert"])
    ftruncate($fo, $fileprop["ssize"]);

  // Restore the header if necessary
  if($header == "UNIF")
  {

    if(file_exists("ninja.src"))
      rebuild_unif($target . $fileprop["name"], file_read("ninja.src"));

    unset($header);
    fclose($fo);

    unlink("ninja.src");

  }
  else if($header)
  {

    if(file_exists("ninja.src"))
      $tfsize = filesize("ninja.src");
    else
      $tfsize = filesize($target . $fileprop["name"]);
  
    fseek($fo, 0, SEEK_SET);
    $temp = fread($fo, $tfsize);
    fseek($fo, 0, SEEK_SET);
    fputs($fo, $header . $temp);

    unset($temp, $header);

  }

  @fclose($fo);

  // If ninja.src exists, we're working on a temp file. Overwrite the original.
  if(file_exists("ninja.src"))
  {

    unlink($target . $fileprop["name"]);
    rename("ninja.src", $target . $fileprop["name"]);

  }

  // Tank our variables
  unset($fileprop, $fo);

  fclose($rup);

  echo("Complete!\r\n");

}


/******************************************************************************
** apply_ips()
*******************************************************************************
** syntax: void apply_ips(string ips_file, string bin_file)
*******************************************************************************
** Applies ips_file to bin_file if both files exist. Compatable with
** IPS RLE and LunarIPS CUT command.
******************************************************************************/

function apply_ips($ips_file, $bin_file)
{

  // Make sure the input is all valid
  if(!file_exists($ips_file))
    die("Error: apply_ips(): Unable to open $ips_file\r\n\r\n");
  if(!file_exists($bin_file))
    die("Error: apply_ips(): Unable to open $bin_file\r\n\r\n");

  // Open the patch file as a stream
  $ips = fopen($ips_file, "rb");

  if(fread($ips, 5) != "PATCH")
    die("Error: apply_ips(): Supplied IPS file does not appear to be an IPS format patch\r\n\r\n");

  echo("Type IPS patch detected!\nApplying $ips_file to $bin_file ...\r\n");

  // Open the binary file in read/write mode
  $fa = fopen($bin_file, "r+b");

  // str("EOF") signals the end of an IPS patch
  $temp = fread($ips, 3);

  while($temp != "EOF" && !feof($ips))
  {

    // Seek to the specified offset in the binary
    fseek($fa, punpack($temp, "b"), SEEK_SET);
    $length = punpack(fread($ips, 2), "b");

    // 0x0 signals an IPS RLE byte

    if($length==0)
    {

      // RLE can be up to 0xffff chars long
      $rle_length = punpack(fread($ips, 2), "b");

      // Character to repeat follows length
      $rle_char = fgetc($ips);

      // Loop the length of the RLE, writing the byte each time
      for($rle_count = 0; $rle_count < $rle_length; $rle_count ++)
        fputs($fa, $rle_char);

    }

    else
      fputs($fa, fread($ips, $length));

    // Read the next offset
    $temp = fread($ips, 3);

  }

  // FuSoYa's LunarIPS program writes beyond EOF to support a "cut" command
  // The offset after EOF is where the file is to be cut at
  if(ftell($ips) < filesize($ips_file))
  {

    echo("Truncating file ...\r\n"); 
    ftruncate($fa, punpack(fread($ips, 3), "b"));

  }

  fclose($fa);
  fclose($ips);

  echo("Complete!\r\n");

}


/******************************************************************************
** apply_ppf()
*******************************************************************************
** syntax: void apply_ppf(string ppf_file, string bin_file)
*******************************************************************************
** Applies ppf_file to bin_file if both files exist.
******************************************************************************/

function apply_ppf($ppf_file, $bin_file)
{

  // Make sure the input is all valid
  if(!file_exists($ppf_file))
    die("Error: apply_ppf(): Unable to open $ppf_file\r\n\r\n");
  if(!file_exists($bin_file))
    die("Error: apply_ppf(): Unable to open $bin_file\r\n\r\n");

  // This is dirty code, it should be fixed later
  $ppfdump = file_read($ppf_file);
  if(strpos($ppfdump, "@BEGIN_FILE_ID"))
    $ppf_end = strpos($ppfdump, "@BEGIN_FILE_ID");
  else
    $ppf_end = filesize($ppf_file);
  unset($ppfdump);

  // Open the patch file as a stream
  $ppf = fopen($ppf_file, "rb");

  if(fread($ppf, 3) != "PPF")
    die("Error: apply_ppf(): Not a valid PPF patch\r\n\r\n");

  $magic = fread($ppf, 2);

  if(($magic == "10" || $magic == "20" || $magic == "30") && ord(fgetc($ppf)) == 0)
  {

    echo("Type PPF 1.0 patch detected!\r\nApplying $ppf_file to $bin_file ...\r\n");

    // Skip PPF internal info
    
    fseek($ppf, 56, SEEK_SET);

    // Open the binary file in read/write mode
    $fa = fopen($bin_file, "r+b");

    while($ftell($ppf) < $ppf_end)
    {

      fseek($fa, punpack(fread($ppf, 4)), SEEK_SET);
      $length = ord(fgetc($fa));
      fputs($fa, fread($ppf, $length));

    }

    fclose($fa);
    fclose($ppf);

    echo("Complete!\r\n");

  }

  else if(($magic == "20" || $magic == "30") && ord(fgetc($ppf)) == 1)
  {

    echo("Type PPF 2.0 patch detected!\r\nApplying $ppf_file to $bin_file ...\r\n");

    // Skip PPF internal info
    
    fseek($ppf, 56, SEEK_SET);

    // Open the binary file in read/write mode
    $fa = fopen($bin_file, "r+b");

    // Run the PPF file size check
    if(punpack(fread($ppf, 4)) != filesize($bin_file))
      die("Error: apply_ppf(): Target file size does not match patch requirement\r\n\r\n");

    // Run the PPF sector match check
    fseek($fa, 0x9320, SEEK_SET);
    $ppf_bin_block = fread($ppf, KBYTE);
    $org_bin_block = fread($fa, KBYTE);

    fseek($fa, 0, SEEK_SET);

    if($ppf_bin_block != $org_bin_block)
      die("Error: apply_ppf(): Target file sector \$9320 does not match patch requirement\r\n\r\n");

    // Unset variables and free up some RAM
    unset($ppf_bin_block, $org_bin_block);

    while($ftell($ppf) < $ppf_end)
    {

      fseek($fa, punpack(fread($ppf, 4)), SEEK_SET);
      $length = ord(fgetc($fa));
      fputs($fa, fread($ppf, $length));

    }

    fclose($fa);
    fclose($ppf);

    echo("Complete!\r\n");

  }

  else if($magic == "30" && ord(fgetc($ppf)) == 2)
  {

    echo("Type PPF 3.0 patch detected!\r\nApplying $ppf_file to $bin_file ...\r\n");

    // Skip PPF internal info
    
    fseek($ppf, 56, SEEK_SET);

    $imagetype = ord(fgetc($ppf));
    $blockcheck = ord(fgetc($ppf));
    $undodata = ord(fgetc($ppf));
    $dummy = fgetc($ppf);

    // Open the binary file in read/write mode
    $fa = fopen($bin_file, "r+b");

    // Run the PPF sector match check
    if($blockcheck != 0)
    {

      if($imagetype == 0)
        fseek($fa, 0x9320, SEEK_SET);
      else if($imagetype == 1)
        fseek($fa, 0x80a0, SEEK_SET);
      $ppf_bin_block = fread($ppf, KBYTE);
      $org_bin_block = fread($fa, KBYTE);

      fseek($fa, 0, SEEK_SET);

      if($ppf_bin_block != $org_bin_block)
        die("Error: apply_ppf(): Target file sector does not match patch requirement\r\n\r\n");

      // Unset variables and free up some RAM
      unset($ppf_bin_block, $org_bin_block);
    }

    while($ftell($ppf) < $ppf_end)
    {

      fseek($fa, punpack(fread($ppf, 8)), SEEK_SET);
      $length = ord(fread($ppf, 2));
      fputs($fa, fread($ppf, $length));

    }

    fclose($fa);
    fclose($ppf);

    echo("Complete!\r\n");

  }

  else
    die("Error: apply_ppf(): Unsupported PPF version\r\n\r\n");

}


/******************************************************************************
** apply_gdiff()
*******************************************************************************
** syntax: void apply_gdiff(string diff_file, string bin_file)
*******************************************************************************
** Applies a Generic Diff Format file. Based on the specification submitted
** to the W3C on August 21, 1997 by Arthur van Hoff and Jonathan Payne of
** Marimba Inc.
******************************************************************************/

function apply_gdiff($diff_file, $bin_file)
{

  // Make sure the input is all valid
  if(!file_exists($diff_file))
    die("Error: apply_gdiff(): Unable to open $diff_file\r\n\r\n");
  if(!file_exists($bin_file))
    die("Error: apply_gdiff(): Unable to open $bin_file\r\n\r\n");

  // Open the patch file as a stream
  $diff = fopen($diff_file, "rb");

  if(fread($ips, 5) != "\xd1\xff\xd1\xff\x04")
    die("Error: apply_gdiff(): Supplied Diff file does not appear to be a Generic Diff Format version 4 patch\r\n\r\n");

  echo("Type Generic Diff Format patch detected!\nApplying $diff_file to $bin_file ...\r\n");

  // Open the binary file in read/write mode
  $fa = fopen($bin_file, "r");
  $fo = fopen("ninja.src", "w");

  // str("EOF") signals the end of an IPS patch
  $temp = ord(fgetc($diff));

  while($temp != 0)
  {

    // Append $temp bytes
    if($temp > 0 && $temp < 247)
      fputs($fo, fread($diff, $temp));

    // Append ushort bytes (16-bit unsigned)
    else if($temp == 247)
    {

      list($temp) = unpack("v", fread($diff, 2));
      fputs($fo, fread($diff, $temp));

    }

    // Append int bytes (32-bit signed)
    else if($temp == 248)
    {

      list($temp) = unpack("V", fread($diff, 4));
      if($temp > 0x7fffffff)
        die("Error: apply_gdiff(): Cannot read negative bytes (-".dechex($temp - 0x7fffffff).")\r\n\r\n");
      fputs($fo, fread($diff, $temp));

    }

    // Copy from ushort offset (16-bit unsigned) ubyte bytes (8-bit unsigned)
    else if($temp == 249)
    {

      list($temp) = unpack("v", fread($diff, 2));
      fseek($fa, $temp, SEEK_SET);
      list($temp) = ord(fgetc($diff));
      fputs($fo, fread($fa, $temp));

    }

    // Copy from ushort offset (16-bit unsigned) ushort bytes (16-bit unsigned)
    else if($temp == 250)
    {

      list($temp) = unpack("v", fread($diff, 2));
      fseek($fa, $temp, SEEK_SET);
      list($temp) = unpack("v", fread($diff, 2));
      fputs($fo, fread($fa, $temp));

    }

    // Copy from ushort offset (16-bit unsigned) int bytes (32-bit signed)
    else if($temp == 251)
    {

      list($temp) = unpack("v", fread($diff, 2));
      fseek($fa, $temp, SEEK_SET);
      list($temp) = unpack("V", fread($diff, 4));
      if($temp > 0x7fffffff)
        die("Error: apply_gdiff(): Cannot read negative bytes (-".dechex($temp - 0x7fffffff).")\r\n\r\n");
      fputs($fo, fread($fa, $temp));

    }

    // Copy from int offset (32-bit signed) ubyte bytes (8-bit unsigned)
    else if($temp == 252)
    {

      list($temp) = unpack("V", fread($diff, 4));
      if($temp > 0x7fffffff)
        die("Error: apply_gdiff(): Cannot read negative bytes (-".dechex($temp - 0x7fffffff).")\r\n\r\n");
      fseek($fa, $temp, SEEK_SET);
      list($temp) = ord(fgetc($diff));
      fputs($fo, fread($fa, $temp));

    }

    // Copy from int offset (32-bit signed) ushort bytes (16-bit unsigned)
    else if($temp == 253)
    {

      list($temp) = unpack("V", fread($diff, 4));
      if($temp > 0x7fffffff)
        die("Error: apply_gdiff(): Cannot read negative bytes (-".dechex($temp - 0x7fffffff).")\r\n\r\n");
      fseek($fa, $temp, SEEK_SET);
      list($temp) = unpack("v", fread($diff, 2));
      fputs($fo, fread($fa, $temp));

    }

    // Copy from int offset (32-bit signed) int bytes (32-bit signed)
    else if($temp == 254)
    {

      list($temp) = unpack("V", fread($diff, 4));
      if($temp > 0x7fffffff)
        die("Error: apply_gdiff(): Cannot read negative bytes (-".dechex($temp - 0x7fffffff).")\r\n\r\n");
      fseek($fa, $temp, SEEK_SET);
      list($temp) = unpack("V", fread($diff, 4));
      if($temp > 0x7fffffff)
        die("Error: apply_gdiff(): Cannot read negative bytes (-".dechex($temp - 0x7fffffff).")\r\n\r\n");
      fputs($fo, fread($fa, $temp));

    }

    // Copy from int offset (32-bit signed) int bytes (32-bit signed)
    else if($temp == 255)
    {

      list($temp) = punpack(fread($diff, 8), 64);
      if(gmp_cmp("$temp", "0x7fffffffffffffff") == 1)
        die("Error: apply_gdiff(): Cannot read negative bytes (-".dechex($temp - 0x7fffffff).")\r\n\r\n");
      fseek($fa, $temp, SEEK_SET);
      list($temp) = unpack("V", fread($diff, 4));
      if($temp > 0x7fffffff)
        die("Error: apply_gdiff(): Cannot read negative bytes (-".dechex($temp - 0x7fffffff).")\r\n\r\n");
      fputs($fo, fread($fa, $temp));

    }

    $temp = ord(fgetc($diff));

  }

  fclose($fa);
  fclose($fo);
  fclose($diff);

  unlink($bin_file);
  rename("ninja.src", $bin_file);

  echo("Complete!\r\n");

}


/******************************************************************************
** apply_fireflower()
*******************************************************************************
** syntax: void apply_fireflower(string fire_file, string bin_file)
*******************************************************************************
** Applies a FireFlower Patch Format file. FireFlower is a popular format
** for NeoGeo graphic and color hacks created by Kuma (kuma_yeb@d1.dion.ne.jp)
** in 2000. Also able to apply console output generated by Microsoft
** File Compare (only when using the /b Binary Compare flag).
******************************************************************************/

function apply_fireflower($fire_file, $bin_file)
{

  // Make sure the input is all valid
  if(!file_exists($fire_file))
    die("Error: apply_fireflower(): Unable to open $fire_file\r\n\r\n");
  if(!file_exists($bin_file))
    die("Error: apply_fireflower(): Unable to open $bin_file\r\n\r\n");

  // Open the patch file as a stream
  $fire = fopen($fire_file, "rb");

  echo("Type FireFlower Patch Format/Microsoft File Compare assumed!\r\nApplying $fire_file to $bin_file ...\r\n\r\nComments:\r\n");

  // Open the binary file in read/write mode
  $fa = fopen($bin_file, "r+b");

  while(!feof($fire))
  {

    $buffer = trim(fgets($fire, 1024));
    if(strlen($buffer) == 15)
    {

      $flags = explode(" ", $buffer);

      if(count($flags) == 3)
      {

        $offset = hexdec(substr($flags[0], 0, 8));
        $src_byte = hexdec($flags[1]);
        $mod_byte = hexdec($flags[2]);

        if(is_int($offset) && is_int($src_byte) && is_int($mod_byte))
        {

          fseek($fa, $offset, SEEK_SET);
          $cur_byte = ord(fgetc($fa));
          fseek($fa, $offset, SEEK_SET);
          if($cur_byte == $src_byte) fputs($fa, chr($mod_byte));
          else if($cur_byte == $mod_byte) fputs($fa, chr($src_byte));
          else echo "Unexpected byte found at 0x" . dechex($offset) . "\r\n";

        }

        else echo "$buffer\r\n";

      }

      else echo "$buffer\r\n";

    }

    else echo "$buffer\r\n";

  }

  fclose($fa);
  fclose($fire);

  echo("Complete!\r\n");

}


/******************************************************************************
** get_info()
*******************************************************************************
** syntax: array get_info(string file)
*******************************************************************************
** Reads a NINJA info file to an array.
******************************************************************************/

function get_info($file)
{

  $fd = fopen($file, "rb");

  $info = array();

  $test = fread($fd, 3);
  if($test != "\xef\xbb\xbf")
  {

    fseek($fd, 0, SEEK_SET);
    $info["enc"] = 0;

  }	  

  else
    $info["enc"] = 1;

  $info["auth"]  = trim(fgets($fd, 4096));
  if(strlen($info["auth"]) > 84) $info["auth"] = substr($info["auth"], 0, 84);

  $info["ver"]   = trim(fgets($fd, 4096));
  if(strlen($info["ver"]) > 11) $info["auth"] = substr($info["ver"], 0, 11);

  $info["title"] = trim(fgets($fd, 4096));
  if(strlen($info["title"]) > 256) $info["title"] = substr($info["title"], 0, 256);

  $info["genre"] = trim(fgets($fd, 4096));
  if(strlen($info["genre"]) > 48) $info["genre"] = substr($info["genre"], 0, 48);

  $info["lang"]  = trim(fgets($fd, 4096));
  if(strlen($info["lang"]) > 48) $info["lang"] = substr($info["lang"], 0, 48);

  $info["year"]  = trim(fgets($fd, 4096));
  if(strlen($info["year"]) > 8) $info["year"] = substr($info["year"], 0, 8);

  $info["web"]   = trim(fgets($fd, 4096));
  if(strlen($info["web"]) > 512) $info["web"] = substr($info["web"], 0, 512);

  $info["desc"]  = trim(fgets($fd, 4096));
  if(strlen($info["desc"]) > 1074) $info["desc"] = substr($info["desc"], 0, 1074);

  fclose($fd);

  return($info);

}


/******************************************************************************
** pxor()
*******************************************************************************
** syntax: string pxor(string string, string xor)
*******************************************************************************
** Returns an XOR string resulting from comparing each byte of string
** against the corresponding byte in xor. If xor is shorter than string it
** will be looped. To use a single byte XOR, like 0x9f, use chr(0x9f) as
** string xor.
*******************************************************************************
** Example: echo pxor("This is a Test", chr(0x20));
** Return : tHIS IS A tEST
******************************************************************************/

function pxor($string, $xor)
{

  if($xor == null || $string == null)
    die("Error: pxor(): Requires a non-null string and non-null xor\r\n\r\n");

  while(strlen($string) > strlen($xor))
    $xor .= $xor;

  $xstring = "";

  for($i = 0; $i < strlen($string); $i ++)
    $xstring .= chr(ord($string[$i]) ^ ord($xor[$i]));

  return($xstring);

}


/******************************************************************************
** ppack()
*******************************************************************************
** syntax: string ppack(string value[, int bits, char endian])
*******************************************************************************
** Packs an integer value to a binary string. If bits is not specified, it is
** assumed to be 16 bit--bits must be a multiple of 8. Endian format is
** assumed to be little endian unless endian equals "b".
*******************************************************************************
** Example: echo bin2hex(ppack(0x19945, 24));
** Return : 459901
******************************************************************************/

function ppack($value, $bits = 16, $endian = "l")
{

  if($bits > 64 || gmp_cmp("$value", "0xffffffffffffffff") > 0)
    die("Error: ppack(): Value is greater than 64 bits\r\n\r\n");
  else if(($bits == 8 && $value > 0xff)||($bits == 16 && $value > 0xffff)||($bits == 24 && $value > 0xffffff)||($bits == 32 && $value > 0xffffffff)||
	  ($bits == 40 && gmp_cmp("$value", "0xffffffffff") > 0)||($bits == 48 && gmp_cmp("$value", "0xffffffffffff") > 0)||($bits == 56 && gmp_cmp("$value", "0xffffffffffffff") > 0)||($bits == 64 && gmp_cmp("$value", "0xffffffffffffffff") > 0))
    die("Error: ppack(): Value passed exceeds specified bits\r\n\r\n");

  if(is_numeric($value) && $value <= 0xffffffff)
  {

    if($endian == "b")
      return(substr(pack("N", $value), 4 - $bits / 8));

    else
      return(substr(pack("V", $value), 0, $bits / 8));

  }

  else if(extension_loaded('gmp'))
  {

    $high = gmp_strval(gmp_div($value, "0x100000000"), 16);
    $low  = gmp_strval(gmp_mod($value, "0x100000000"), 16);

    if(strlen($high) % 2 > 0)
      $high = "0" . $high;
    
    if($endian == "b")
      return(substr(pack("H*", $high) . pack("H*", $low), 8 - $bits / 8));

    else
      return(substr(strrev(pack("H*", $low)) . strrev(pack("H*", $high)), 0, $bits / 8));

  }

  else
    die("Error: ppack(): GMP is required for integers over 32-bits\r\n\r\n");

}


/******************************************************************************
** punpack()
*******************************************************************************
** syntax: int punpack(string value[, char endian])
*******************************************************************************
** Unpacks a binary string value to a decimal. Endian format is assumed to
** be little endian unless endian equals "b".
*******************************************************************************
** Example: echo punpack(ppack(0xedbeef, 24));
** Return : 15580911
******************************************************************************/

function punpack($value, $endian = "l")
{

  if($endian != "b")
    $value = strrev($value);

  if(strlen($value) <= 4)
    return(hexdec(bin2hex($value)));

  else if(extension_loaded('gmp'))
  {

    $low  = hexdec(bin2hex((substr($value, -4))));
    $high = hexdec(bin2hex((substr($value, 0, strlen($value) - 4))));

    return(gmp_strval(gmp_add(gmp_mul("$high", "0x100000000"), "$low")));

  }

  else
    die("Error: punpack(): GMP is required for integers over 32-bits\r\n\r\n");

}


/******************************************************************************
** list_dir()
*******************************************************************************
** syntax: array list_dir(string path[, string mask, char slash, bool recurse])
*******************************************************************************
** Returns an array with complete paths to all files in path. Omits the
** entries "." and "..". If slash is not specified, "/" will be used. You
** may specify a mask using "*" at the start or end of the string as
** wildcards. For example, "*.php" matches all files ending in ".php",
** "php*" will match all files starting with PHP. "*php*" or "php" will
** match all files containing "php" anywhere in their filenames.
******************************************************************************/

function list_dir($path, $mask = "*.*", $slash = "/", $recurse = FALSE)
{

  if($mask[0] == "*" && $mask[(strlen($mask) - 1)] == "*")
    $regex = substr($mask, 1, strlen($mask) - 2);
  else if($mask[0] == "*")
    $regex = substr($mask, 1) . "$";
  else if($mask[(strlen($mask) - 1)] == "*")
    $regex = "^" . substr($mask, 0, strlen($mask) - 1);
  else
    $regex = $mask;

  $files = array();

  if($ls_out = opendir($path))
  {

    while(($file = readdir($ls_out)))
    {

      if($file != "." && $file != ".." && ereg($regex, $file))
      {

        if(is_dir("$path$slash$file") && $recurse)
        {

          $files += list_dir("$path$slash$file", $mask, $slash, TRUE);
          continue;

	}

        $files[] = "$path$slash$file";

      }

    }

    closedir($ls_out);

  }

  else
    die("Error: list_dir(): Unable to open $path\r\n\r\n");

  return($files);

}


/******************************************************************************
** file_read()
*******************************************************************************
** syntax: string file_read(string filename[, int length, int offset])
*******************************************************************************
** Reads a section of length bytes long filename starting at offset to a raw
** string. If length and offset are omitted, the entire file is read.
******************************************************************************/

function file_read($filename, $length = null, $offset = 0)
{

  if(!file_exists($filename))
    die("Error: file_read(): Unable to open $filename\r\n\r\n");

  if($length == null)
    $length = filesize($filename);

  $fd = fopen($filename, "rb");
  fseek($fd, $offset, SEEK_SET);
  $fddump = fread($fd, $length);

  fclose($fd);
  return($fddump);

}


/******************************************************************************
** smd_deint()
*******************************************************************************
** syntax: string smd_deint(string chunk)
*******************************************************************************
** Takes a 16KB block of SMD data chunk and returns it after deinterleaving.
******************************************************************************/

function smd_deint($chunk)
{

  if(strlen($chunk) != 16 * KBYTE)
    die("Error: smd_deint(): Chunk does not equal 16KB\r\n\r\n");

  $low   = 1;
  $high  = 0;
  $block = array();

  for($i = 0; $i < 8 * KBYTE; $i ++)
  {

    $block[$low]  = $chunk[(8 * KBYTE + $i)];
    $block[$high] = $chunk[$i];

    $low  += 2;
    $high += 2;

  }

  return(implode("", $block));

}


/******************************************************************************
** sfam_read()
*******************************************************************************
** syntax: string sfam_read(string filename)
*******************************************************************************
** Reads a Super Famicom/Nintendo ROM to a string and deinterleaves it if
** not already deinterleaved. Returns the ROM as a string.
******************************************************************************/

function sfam_read($infile)
{

  if(!file_exists($infile))
    die("Error: sfam_read(): Unable to open $infile\r\n\r\n");

  $fddump = file_read($infile);
  $header = "";

  // "SUPERUFO"
  $ufotest = substr($fddump, 0x8, 8);
  // "GAME DOCTOR SF 3"
  $gd3test = substr($fddump, 0, 0x10);

  if(substr($fddump, 0x1e8, 4) == "NSRT")
    $header = substr($fddump, 0, SNES_HEADER);

  if(strlen($fddump) % (32 * KBYTE) != 0)
    $fddump = substr($fddump, SNES_HEADER);

  $test = array();
  $test['inverse']  = punpack(substr($fddump, 0x7fdc, 2));
  $test['checksum'] = punpack(substr($fddump, 0x7fde, 2));
  $test['romstate'] = ord(substr($fddump, 0x7fd5, 1)) % 0x10;

  if($test['inverse'] + $test['checksum'] == 0xffff && $test['romstate'] % 2 == 0)
  {

    echo("ROM type LoROM detected\r\n");
    return(array($fddump, $header));

  }

  else if($test['inverse'] + $test['checksum'] == 0xffff && $test['romstate'] % 2 != 0)
  {

    $chart_20mbit = array(
        1,   3,   5,   7,   9,  11,  13,  15,  17,  19,  21,  23,  25,  27,  29,
       31,  33,  35,  37,  39,  41,  43,  45,  47,  49,  51,  53,  55,  57,  59,
       61,  63,  65,  67,  69,  71,  73,  75,  77,  79,  64,  66,  68,  70,  72,
       74,  76,  78,  32,  34,  36,  38,  40,  42,  44,  46,  48,  50,  52,  54,
       56,  58,  60,  62,   0,   2,   4,   6,   8,  10,  12,  14,  16,  18,  20,
       22,  24,  26,  28,  30
    );

    $chart_24mbit = array(
        1,   3,   5,   7,   9,  11,  13,  15,  17,  19,  21,  23,  25,  27,  29,
       31,  33,  35,  37,  39,  41,  43,  45,  47,  49,  51,  53,  55,  57,  59,
       61,  63,  65,  67,  69,  71,  73,  75,  77,  79,  81,  83,  85,  87,  89,
       91,  93,  95,  64,  66,  68,  70,  72,  74,  76,  78,  80,  82,  84,  86,
       88,  90,  92,  94,   0,   2,   4,   6,   8,  10,  12,  14,  16,  18,  20,
       22,  24,  26,  28,  30,  32,  34,  36,  38,  40,  42,  44,  46,  48,  50,
       52,  54,  56,  58,  60,  62
    );

    $chart_48mbit = array(
      129, 131, 133, 135, 137, 139, 141, 143, 145, 147, 149, 151, 153, 155, 157,
      159, 161, 163, 165, 167, 169, 171, 173, 175, 177, 179, 181, 183, 185, 187,
      189, 191, 128, 130, 132, 134, 136, 138, 140, 142, 144, 146, 148, 150, 152,
      154, 156, 158, 160, 162, 164, 166, 168, 170, 172, 174, 176, 178, 180, 182,
      184, 186, 188, 190,   1,   3,   5,   7,   9,  11,  13,  15,  17,  19,  21,
       23,  25,  27,  29,  31,  33,  35,  37,  39,  41,  43,  45,  47,  49,  51,
       53,  55,  57,  59,  61,  63,  65,  67,  69,  71,  73,  75,  77,  79,  81,
       83,  85,  87,  89,  91,  93,  95,  97,  99, 101, 103, 105, 107, 109, 111,
      113, 115, 117, 119, 121, 123, 125, 127,   0,   2,   4,   6,   8,  10,  12,
       14,  16,  18,  20,  22,  24,  26,  28,  30,  32,  34,  36,  38,  40,  42,
       44,  46,  48,  50,  52,  54,  56,  58,  60,  62,  64,  66,  68,  70,  72,
       74,  76,  78,  80,  82,  84,  86,  88,  90,  92,  94,  96,  98, 100, 102,
      104, 106, 108, 110, 112, 114, 116, 118, 120, 122, 124, 126
    );

    if(strlen($fddump) == 512 * KBYTE * 5 && $ufotest != "SUPERUFO")
    {

      $chunks = array();
      
      for($i = 0; $i < 0x50; $i ++)
        $chunks[$i] = substr($fddump, $i * (32 * KBYTE), (32 * KBYTE));

      $deinterleave = "";

      for($i = 0; $i < 0x50; $i ++)
        $deinterleave .= $chunks[array_search($i, $chart_20mbit)];

      echo("ROM type HiROM interleaved detected\r\n");
      return(array($deinterleave, $header));

    }

    else if(strlen($fddump) == 512 * KBYTE * 6 && $ufotest != "SUPERUFO")
    {

      $chunks = array();

      for($i = 0; $i < 0x60; $i ++)
        $chunks[$i] = substr($fddump, $i * (32 * KBYTE), (32 * KBYTE));

      $deinterleave = "";

      for($i = 0; $i < 0x60; $i ++)
        $deinterleave .= $chunks[array_search($i, $chart_24mbit)];

      echo("ROM type HiROM interleaved detected\r\n");
      return(array($deinterleave, $header));

    }

    else if(strlen($fddump) == 512 * KBYTE * 12 && $ufotest != "SUPERUFO")
    {

      $chunks = array();

      for($i = 0; $i < 0xc0; $i ++)
        $chunks[$i] = substr($fddump, $i * (32 * KBYTE), (32 * KBYTE));

      $deinterleave = "";

      for($i = 0; $i < 0xc0; $i ++)
        $deinterleave .= $chunks[array_search($i, $chart_48mbit)];

      echo("ROM type HiROM interleaved detected\r\n");
      return(array($deinterleave, $header));

    }

    else
    {

      $chunks = strlen($fddump) / (32 * KBYTE);
      $deinterleave = "";

      for($i = 0; $i < ($chunks / 2); $i ++)
      {

	$deinterleave .= substr($fddump, ($i + ($chunks / 2)) * (32 * KBYTE), (32 * KBYTE));
        $deinterleave .= substr($fddump, $i * (32 * KBYTE), (32 * KBYTE));

      }

      echo("ROM type HiROM interleaved detected\r\n");
      return(array($deinterleave, $header));

    }

  }

  $test['inverse']  = punpack(substr($fddump, 0xffdc, 2));
  $test['checksum'] = punpack(substr($fddump, 0xffde, 2));
  $test['romstate'] = ord(substr($fddump, 0xffd5, 1)) % 0x10;

  if($test['inverse'] + $test['checksum'] == 0xffff && $test['romstate'] % 2 != 0)
  {

    echo("ROM type HiROM deinterleaved detected\r\n");
    return(array($fddump, $header));

  }

  else if($test['romstate'] % 2 != 0)
  {

    echo("ROM type uncertain. Possibly a HiROM beta or unreleased game ...\r\n");
    return(array($fddump, $header));

  }

  else
    die("Error: sfam_read(): Supplied ROM file does not appear to be Super Nintendo ROM\r\n\r\n");

}


/******************************************************************************
** sms_read()
*******************************************************************************
** syntax: string sms_read(string filename)
*******************************************************************************
** Reads a Sega Master System ROM to a string and deinterleaves it if
** not already in binary format. Returns the ROM as a string.
******************************************************************************/

function sms_read($infile)
{

  if(!file_exists($infile))
    die("Error: sms_read(): Unable to open $infile\r\n\r\n");

  $fd = fopen($infile, "rb");
  fseek($fd, 0x7ff4, SEEK_SET);

  // Test if the file is in BIN format
  if(fread($fd, 4) == "SEGA")
  {

    fseek($fd, 0, SEEK_SET);
    return(fread($fd, filesize($infile)));

  }

  fseek($fd, 0x8, SEEK_SET);

  // Test if the file is in SMD format
  if(fread($fd, 2) == "\xaa\xbb")
  {

    fseek($fd, 0x200, SEEK_SET);
    $num_blocks = (filesize($infile) - 0x200) / (16 * KBYTE);
    $output = "";

    for($i = 0; $i < $num_blocks; $i ++)
      $output.= smd_deint(fread($fd, 16 * KBYTE));

    return($output);

  }

  else
    die("Error: sms_read(): Supplied ROM file does not appear to be Megadrive ROM\r\n\r\n");

}


/******************************************************************************
** mega_read()
*******************************************************************************
** syntax: string mega_read(string filename)
*******************************************************************************
** Reads a Sega Megadrive/Genesis ROM to a string and deinterleaves it if
** not already in binary format. Returns the ROM as a string.
******************************************************************************/

function mega_read($infile)
{

  if(!file_exists($infile))
    die("Error: mega_read(): Unable to open $infile\r\n\r\n");

  $fd = fopen($infile, "rb");
  fseek($fd, 0x100, SEEK_SET);

  // Test if the file is in BIN format
  if(fread($fd, 4) == "SEGA")
  {

    fseek($fd, 0, SEEK_SET);
    return(fread($fd, filesize($infile)));

  }

  fseek($fd, 0x8, SEEK_SET);

  // Test if the file is in SMD format
  if(fread($fd, 2) == "\xaa\xbb")
  {

    fseek($fd, 0x200, SEEK_SET);
    $num_blocks = (filesize($infile) - 0x200) / (16 * KBYTE);
    $output = "";

    for($i = 0; $i < $num_blocks; $i ++)
      $output .= smd_deint(fread($fd, 16 * KBYTE));

    fseek($fd, 0, SEEK_SET);

    return($output);

  }

  else
    die("Error: mega_read(): Supplied ROM file does not appear to be Megadrive ROM\r\n\r\n");

}


/******************************************************************************
** gb_read()
*******************************************************************************
** syntax: string gb_read(string filename)
*******************************************************************************
** Reads a Game Boy ROM to a string and strips the header if needed.
******************************************************************************/

function gb_read($infile)
{

  $fd = fopen($infile, "rb");

  // Test for an 0x200 SmartCard header
  if((filesize($infile) % 0x4000) != 0)
  {

    fseek($fd, 0x200, SEEK_SET);
    $rom = fread($fd, (filesize($infile) - 0x200));

  }

  else
    $rom = fread($fd, filesize($infile));

  fclose($fd);

  // Magic byte suggested by Aerdan. Does this cause unlicensed games to fail?
  if(substr($rom, 0x104, 4) != "\xce\xed\x66\x66")
    die("Error: gb_read(): Supplied ROM file does not appear to be Game Boy ROM\r\n\r\n");

  return($rom);

}


/******************************************************************************
** nes_read()
*******************************************************************************
** syntax: string nes_read(string filename, bool TYPE)
*******************************************************************************
** Reads a Nintendo 8-bit ROM to a string and strips the header
** if needed.
******************************************************************************/

function nes_read($infile)
{

  $fd = fopen($infile, "rb");

  $test1 = fread($fd, 3);
  $test3 = fgetc($fd);

  fseek($fd, 0x8, SEEK_SET);
  $test2 = fread($fd, 2);

  $header = "";

  fseek($fd, 0, SEEK_SET);

  // Check for an iNES header.
  if($test1 == "NES")
  {

    $header = fread($fd, 0x10);
    $rom = fread($fd, (filesize($infile) - 0x10));

  }

  // Check for a Far Front East header.
  else if($test2 == "\xaa\xbb")
  {

    $header = fread($fd, 0x200);
    $rom = fread($fd, (filesize($infile) - 0x200));

  }

  // Check for a UNIF format ROM
  else if($test1 . $test3 == "UNIF")
  {

    fseek($fd, 0x20, SEEK_SET);
    $merged_rom = "";

    $command = fread($fd, 4);

    while(ftell($fd) < filesize($infile))
    {
	    
      if(
        $command == "PRG0" || $command == "PRG1" || $command == "PRG2" || $command == "PRG3" || $command == "PRG4" || $command == "PRG5" || $command == "PRG6" || $command == "PRG7" ||
        $command == "PRG8" || $command == "PRG9" || $command == "PRGA" || $command == "PRGB" || $command == "PRGC" || $command == "PRGD" || $command == "PRGE" || $command == "PRGF"
      )
      {

        $size = punpack(fread($fd, 4));
        $merged_rom .= fread($fd, $size);

      }

      else if(
        $command == "CHR0" || $command == "CHR1" || $command == "CHR2" || $command == "CHR3" || $command == "CHR4" || $command == "CHR5" || $command == "CHR6" || $command == "CHR7" ||
        $command == "CHR8" || $command == "CHR9" || $command == "CHRA" || $command == "CHRB" || $command == "CHRC" || $command == "CHRD" || $command == "CHRE" || $command == "CHRF"
      )
      {

        $size = punpack(fread($fd, 4));
        $merged_rom .= fread($fd, $size);

      }

      else
      {

        $size = punpack(fread($fd, 4));
        $dead = fread($fd, $size);

      }

      if(ftell($fd) < filesize($infile))
        $command = fread($fd, 4);

    }

    return(array($merged_rom, "UNIF"));

  }

  else
    die("Error: nes_read(): Supplied ROM file is not a supported ROM format\r\n\r\n");

  fclose($fd);

  return(array($rom, $header));

}


/******************************************************************************
** rebuild_unif()
*******************************************************************************
** syntax: void rebuild_unif(string filename, string data)
*******************************************************************************
** Inserts raw NES PRG+CHR data back into a UNIF format NES ROM.
******************************************************************************/

function rebuild_unif($infile, $data)
{

  $fd = fopen($infile, "r+b");

  fseek($fd, 0x20, SEEK_SET);

  $command = fread($fd, 4);

  while(ftell($fd) < filesize($infile))
  {
	    
    if(
      $command == "PRG0" || $command == "PRG1" || $command == "PRG2" || $command == "PRG3" || $command == "PRG4" || $command == "PRG5" || $command == "PRG6" || $command == "PRG7" ||
      $command == "PRG8" || $command == "PRG9" || $command == "PRGA" || $command == "PRGB" || $command == "PRGC" || $command == "PRGD" || $command == "PRGE" || $command == "PRGF"
    )
    {

      $size = punpack(fread($fd, 4));
      fputs($fd, substr($data, 0, $size));
      $data = substr($data, $size);

    }

    else if(
      $command == "CHR0" || $command == "CHR1" || $command == "CHR2" || $command == "CHR3" || $command == "CHR4" || $command == "CHR5" || $command == "CHR6" || $command == "CHR7" ||
      $command == "CHR8" || $command == "CHR9" || $command == "CHRA" || $command == "CHRB" || $command == "CHRC" || $command == "CHRD" || $command == "CHRE" || $command == "CHRF"
    )
    {

      $size = punpack(fread($fd, 4));
      fputs($fd, substr($data, 0, $size));
      $data = substr($data, $size);

    }

    else
    {

      $size = punpack(fread($fd, 4));
      $dead = fread($fd, $size);

    }

    if(ftell($fd) < filesize($infile))
      $command = fread($fd, 4);

  }

  fclose($fd);

}


/******************************************************************************
** lynx_read()
*******************************************************************************
** syntax: string lynx_read(string filename, bool TYPE)
*******************************************************************************
** Reads a PC-Engine ROM to a string and strips the header and deinterleaves
** if needed.
******************************************************************************/

function lynx_read($infile)
{

  $fd = fopen($infile, "rb");

  $test = fread($fd, 4);
  $header = "";

  fseek($fd, 0, SEEK_SET);

  // Check for a LYNX header.
  if($test == "LYNX")
  {

    $header = fread($fd, 0x40);
    $rom = fread($fd, (filesize($infile) - 0x40));

  }

  else
    $rom = fread($fd, filesize($infile));

  fclose($fd);

  return(array($rom, $header));

}


/******************************************************************************
** pce_read()
*******************************************************************************
** syntax: string pce_read(string filename)
*******************************************************************************
** Reads a PC-Engine ROM to a string and strips the header and deinterleaves
** if needed.
******************************************************************************/

function pce_read($infile)
{

  $fd = fopen($infile, "rb");

  // Test for an 0x200 SmartCard header
  if((filesize($infile) % 0x1000) != 0)
  {

    fseek($fd, 0x200, SEEK_SET);
    $rom = fread($fd, (filesize($infile) - 0x200));

  }

  else
    $rom = fread($fd, filesize($infile));

  fclose($fd);

  return($rom);

}


/******************************************************************************
** n64_read()
*******************************************************************************
** syntax: string n64_read(string filename)
*******************************************************************************
** Reads a Nintendo ROM to a string and deinterleaves if needed.
******************************************************************************/

function n64_read($infile)
{

  $fd = fopen($infile, "rb");

  $test = fread($fd, 4);
  fseek($fd, 0x0, SEEK_SET);

  $deinterleaved = "\x80\x37\x12\x40";
  $interleaved   = "\x37\x80\x40\x12";

  $rom = "";

  // Test for interleave
  if($test == $deinterleaved)
    $rom = fread($fd, filesize($infile));

  else if($test == $interleaved)
  {

    for($i = 0; $i < filesize($infile) / 2; $i ++)
    {
	    
      $char1 = fgetc($fd);
      $char2 = fgetc($fd);
      $rom .= $char2 . $char1;

    }

  }

  else
    die("Error: n64_read(): Supplied ROM file does not appear to be Nintendo 64 ROM\r\n\r\n");

  fclose($fd);

  return($rom);

}


/******************************************************************************
** main()
*******************************************************************************
** syntax: void main()
******************************************************************************/

function main($argc, $argv)
{

  // Make sure we don't have any lingering backup files
  if(file_exists("ninja.src")) unlink("ninja.src");
  if(file_exists("ninja.mod")) unlink("ninja.mod");

  $version = "2.0.0";
  echo("NINJA v$version (C)2006 Derrick Sobodash\r\n\r\n");
  set_time_limit(0x600000);

  // Check the PHP version of the user
  if(phpversion() < "5")
    die("Error: You must be running PHP version 5.0 or greater! Please upgrade.\r\n  Newer versions can be obtained for free from http://www.php.net/\r\n\r\n");

  if($argc == 3)
  {

    list($killit, $patch, $target) = $argv;
    if(!file_exists($patch))
      die("Error: Patch file not found\r\n\r\n");

  }

  else if($argc > 3 && $argc < 7)
  {

    list($killit, $source, $modified, $target, $type, $info) = $argv;

    if(!file_exists($source) && !file_exists($modified))
      die("Error: Source or modified not found\r\n\r\n");

  }

  else
    die("Bad command line!\r\n\r\nCreate Patch:\r\n  ninja2.php source_file modified_file target_file format info_file\r\nApply Patch:\r\n  ninja2.php patch_file target_filer\r\n\r\nSource, modified and target can be either directories or files.\r\n\r\n");

  unset($killit);

  if($patch)
  {

    if(!is_writable($target))
      die("Target file is not writeable!\r\n\r\nPlease check to make sure the file is not marked Read-Only and that you have permissions to modify this file.\r\n\r\n");

    // Read the file tag and version
    $fd = fopen($patch, "rb");
    $fileid  = fread($fd, 5);
    $filever = fgetc($fd);
    fclose($fd);

    // IPS File
    if($fileid == "PATCH")
      apply_ips($patch, $target);

    // PPF File
    else if($fileid == "PPF10" || $fileid == "PPF20" || $fileid == "PPF30")
      apply_ppf($patch, $target);

    // GDIFF File
    else if($fileid == "\xd1\xff\xd1\xff\x04")
      apply_gdiff($patch, $target);

    // XDELTA File
    else if($fileid == "\x25\x58\x44\x5a\x30")
    {

      if(!file_exists("xdelta.exe"))
        die("Error: xdelta binary is unavailable\r\n\r\n");
      else
      {

        echo("Outputting to $target-patched...\r\n\r\n");

        exec("xdelta.exe patch -V \"$patch\" \"$target\" \"$target-patched\"");
	if(filesize("$target-patched") > 0)
          echo("The file appears to have been patched successfully. However, errors are still possible if your MD5 sum did not match the expected value.\r\n\r\nIf you wish to be sure, you should run xdelta.exe from the command line. Its MD5 notices are not printed to the console in a way that can be trapped and displayed here.");
        else
	{

          echo("File patching failed.\r\n\r\nMost likely, this was cause by your file being a different file size than that which is expected in the xdelta patch.");
          unlink("$target-patched");

	}

      }

    }

    // NINJA File
    else if($fileid =="NINJA")
    {

      if($filever == "2")
        apply_ninja2($patch, $target);
      else
        die("Error: Unsupported NINJA patch version\r\n\r\n");

    }

    // Unrecognized or FireFlower
    else
    {

      $fd = fopen($patch, "rb");
      $fileid  = explode("\r\n", fread($fd, filesize($patch)));
      fclose($fd);
      $is_fireflower = 0;

      for($i = 0; $i < count($fileid); $i ++)
      {

        if(strlen(trim($fileid[$i])) == 15)
        {

          $flags = explode(" ", trim($fileid[$i]));
          $offset = hexdec(substr($flags[0], 0, 8));
          $src_byte = hexdec($flags[1]);
          $mod_byte = hexdec($flags[2]);

          if(is_int($offset) && is_int($src_byte) && is_int($mod_byte))
          {

            $is_fireflower = 1;
            break;

          }

        }

      }

      if($is_fireflower == 1)
        apply_fireflower($patch, $target);

      else
        die("Error: Unsupported patch format\r\n\r\n"); 

    }

  }

  else if($modified)
  {

    $patch_info = null;

    if(file_exists($info))
      $patch_info = get_info($info);

    /************************************************************************
    ** NINJA 2.0 Data Types
    *************************************************************************
    **  0 - Raw Binary                    5 - Nintendo Game Boy
    **  1 - Nintendo/Famicom              6 - Sega Master System/Game Gear
    **  2 - Famicom Disk System           7 - Sega Genesis/Megadrive
    **  3 - Super Nintendo/Famicom        8 - NEC TurboGrafx-16/PC-Engine
    **  4 - Nintendo 64                   9 - Atari Lynx
    ************************************************************************/

    $data_types = array("raw", "nes", "fds", "snes", "n64", "gb", "sms", "mega", "pce", "lynx");

    $type = array_search($type, $data_types);

    create_ninja2($source, $modified, $target, $type, $patch_info);

  }

  else
    die("Error: Unknown error in main()\r\n\r\n");

}


main($argc, $argv);



/******************************************************************************
**
** SOURCE CODE ENDS
**
******************************************************************************/


?>
