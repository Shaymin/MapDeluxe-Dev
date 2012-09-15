<?php

$fo = fopen($argv[2], "w");

if(!$argv[1])
{

  fputs($fo, "No patch file loaded!");
  fclose($fo);
  die;

}

$fd = fopen($argv[1], "rb");
$test1 = fread($fd, 3);
$test3 = fread($fd, 2);
$test2 = fgetc($fd);

if($test1 == "PPF" && ($test3 == "10" || $test3 == "20" || $test3 =="30"))
{

  $info = rtrim(fread($fd, 50), "\0 ");
  if($info)
    fputs($fo, "Description:\r\n\r\n$info");
  fseek($fd, 0, SEEK_SET);
  $lala = fread($fd, filesize($argv[1]));

  if(strpos($lala, "@BEGIN_FILE_ID.DIZ"))
  {

    if($info)
      fputs($fo, "\r\n\r\nFILE_ID.DIZ:\r\n\r\n".substr($lala, strpos($lala, "@BEGIN_FILE_ID.DIZ")+18, (ord($lala[strlen($lala)-3])*0x100)+ord($lala[strlen($lala)-4])));
    else
      fputs($fo, "FILE_ID.DIZ:\r\n\r\n".substr($lala, strpos($lala, "@BEGIN_FILE_ID.DIZ")+18, (ord($lala[strlen($lala)-3])*0x100)+ord($lala[strlen($lala)-4])));
  
  }

  fclose($fd);

}

else if($test1 . $test3 == "NINJA" && $test2 == "2")
{

  $enc   = ord(fgetc($fd));
  $auth  = rtrim(fread($fd, 84), "\0");
  $ver   = rtrim(fread($fd, 11), "\0");
  $title = rtrim(fread($fd, 256), "\0");
  $genre = rtrim(fread($fd, 48), "\0");
  $lang  = rtrim(fread($fd, 48), "\0");
  $year  = trim(fread($fd, 4));
  $month = trim(fread($fd, 2));
  $day   = trim(fread($fd, 2));
  if($year || $month || $day)
    $date = date("F j, Y", mktime(0, 0, 0, $month, $day, $year));
  else
    $date = "";
  $web   = rtrim(fread($fd, 512), "\0");
  $desc  = str_replace("\\n", "\r\n", rtrim(fread($fd, 1074), "\0"));
  fclose($fd);

  if($enc == 1)
    fputs($fo, chr(0xef) . chr(0xbb) . chr(0xbf));

  if(strlen($auth) > 0)
    fputs($fo, "Author:       $auth\r\n");
  if(strlen($ver) > 0)
    fputs($fo, "Version:      $ver\r\n");
  if(strlen($title) > 0)
    fputs($fo, "Title:        $title\r\n");
  if(strlen($genre) > 0)
    fputs($fo, "Genre:        $genre\r\n");
  if(strlen($lang) > 0)
    fputs($fo, "Language:     $lang\r\n");
  if(strlen($date) > 0)
    fputs($fo, "Release Date: $date\r\n");
  if(strlen($web) > 0)
    fputs($fo, "Website:      $web\r\n");
  if(strlen($desc) > 0)
    fputs($fo, "\r\nDescription:\r\n\r\n$desc");

}

else if($test1 == chr(0x25) . chr(0x58) . chr(0x44))
{

  fclose($fd);
  fputs($fo, "xdelta has the following to report:\r\n\r\n");

  if(!file_exists("xdelta.exe"))
    fputs($fo, "xdelta.exe not found!");

  else
  {

    exec("xdelta.exe info \"" . $argv[1] . "\"", $result);
    $null = array_pop($result);  $null = array_pop($result);  $null = array_pop($result);
    $result = str_replace("xdelta.exe: ", "", implode("\r\n", $result));
    fputs($fo, $result);

  }

}

else if($test1 . $test3 == "PATCH")
  fputs($fo, "IPS: No internal information!");

else if($test1 . $test3 == chr(0xd1) . chr(0xff) . chr(0xd1) . chr(0xff) . chr(0x4))
  fputs($fo, "Generic Diff: No internal information!");

else
{

  fseek($fd, 0, SEEK_SET);
  $is_fireflower = 0;
  $output = "";

  while(!feof($fd))
  {

    $buffer = trim(fgets($fd, 1024));

    if(strlen($buffer) == 15)
    {

      $flags = explode(" ", $buffer);

      if(count($flags) == 3)
      {

        $offset = hexdec(substr($flags[0], 0, 8));
        $src_byte = hexdec($flags[1]);
        $mod_byte = hexdec($flags[2]);

        if(is_int($offset) && is_int($src_byte) && is_int($mod_byte))
          $is_fireflower = 1;

        else $output .= "$buffer\r\n";

      }

      else $output .= "$buffer\r\n";

    }

    else $output .= "$buffer\r\n";

  }

  if($is_fireflower == 1)
    fputs($fo, "Comments:\r\n\r\n$output");
  else
    fputs($fo, "No internal information!");
  fclose($fd);

}

fclose($fo);

?>
