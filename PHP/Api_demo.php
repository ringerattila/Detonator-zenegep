<?php if (!defined('BASEPATH')) exit('No direct script access allowed');

class Api extends CI_Controller
{

  public function __construct()
  {
    parent::__construct();

    // Load form helper library
    $this->load->helper('form');
    $this->load->helper('url');

    // Load form helper library
    $this->load->helper('detonator');

    // Load form validation library
    $this->load->config('detonator');

    // Load database
    $this->load->model('zenegepek_model');
    $this->load->model('mobil_model');

    // Kliens platform megállapításához
    $this->load->library('user_agent');

    // Load database
    //$this->load->model('ugyek_model');
  }

  // Show view Page
  public function index()
  {
    $data['pageheading'] = 'Detonator Gépkezelő API';
    $this->load->view("api1", $data);
    //  $this->load->view("DataTypes_start");
  }

  // Show view Page
  public function albumkodok()
  {
    $data['pageheading'] = 'Detonator Gépkezelő API';

    $work_place = $this->config->config['work_place'];
    //	$work_place = 'szerver';
    if ($work_place == 'local') {
      $albumKodok = array(
        0 => array('albumkod' => '81001'),
        1 => array('albumkod' => '81002'),
        2 => array('albumkod' => '81003'),
        3 => array('albumkod' => '81004'),
        4 => array('albumkod' => '81005'),
      );
    } else {
      $albumKodok = $this->zenegepek_model->albums_check();
    }
    echo json_encode($albumKodok);
  }


  public function album_check_dir($albumkod)
  {
    $checkPath = $this->config->config['album_url'];
    $data = albums_check_dir($checkPath, $albumkod);
    echo json_encode($data);
  }

  public function album_dirlist($albumkod)
  {
    $checkPath = $this->config->config['album_url'];
    $data = scandir($checkPath);
    //	$data = albums_check_dir($checkPath, $albumkod);
    echo json_encode($data);
  }


  public function is_dir_in_database($albumkod)
  {
    $checkPath = $this->config->config['album_url'];
    $SQLresult = $this->zenegepek_model->albums_read($albumkod);
    $myLength = count($SQLresult);

    if ($myLength == 1) {
      $data['indatabase'] = 'OK';
    } else {
      $data['indatabase'] = 'Nincs';
    }

    echo json_encode($data);
  }


  /* A paraméterként kapott albumkód (könyvtár) alapján a kívánt könyvtárból
a bitton.bmpr és a cover.bmp file-okat a media/image könyvtárba másolja, miután átnevezte 
a bittonALBUMKOD.bmp és coverALBUMKOD.bmp nevekre. Ezáltal a media/image könyvtárban
egyszerre ott lesz az összes cover és bitton file, lehet velük csoportos műveletet végezni.
*/
  public function bmp_copy($albumkod)
  {
    $checkPath = $this->config->config['media_path'];
    $sourcePath = $checkPath . 'albums/' . $albumkod . '/bitmap/bitton.bmp';
    if (file_exists($sourcePath)) {
      $targetPath = $checkPath . 'images/bitton' . $albumkod . '.bmp';
      $copyBittonResult = copy($sourcePath, $targetPath);
    } else {
      $copyBittonResult = false;
    }

    $copyBmpCoverResult = false;
    $copyJpg = false;
    $sourcePath = $checkPath . 'albums/' . $albumkod . '/bitmap/cover.bmp';
    if (file_exists($sourcePath)) {
      $targetPath = $checkPath . 'images/cover' . $albumkod . '.bmp';
      $copyBmpCoverResult = copy($sourcePath, $targetPath);
    } else {
      $sourcePath = $checkPath . 'albums/' . $albumkod . '/bitmap/cover.jpg';
      if (file_exists($sourcePath)) {
        $targetPath = $checkPath . 'images/cover' . $albumkod . '.jpg';
        $copyCoverResult = copy($sourcePath, $targetPath);
        $copyJpg = true;
      }
    }


    if ($copyBittonResult) {
      $data['result'] = $albumkod . ' bitton - OK';
    } else {
      $data['result'] = $albumkod . ' bitton - Hiba';
    }


    if ($copyBmpCoverResult) {
      $data['result'] .= '   |   ' . $albumkod . ' BMP cover - OK';
    } else {
      $data['result'] .=  '   |   ' . $albumkod . ' BMP cover - Hiba';
    }

    if (!$copyBmpCoverResult) {
      if ($copyJpg) {
        $data['result'] .= '   |   ' . $albumkod . ' JPG cover - OK';
      } else {
        $data['result'] .=  '   |   ' . $albumkod . ' JPG cover - Hiba';
      }
    }

    echo json_encode($data);
  }


  // Youtube zenék ellenőrzése
  public function yt_filenames_list($page)
  {
    $data['fileNames'] = $this->zenegepek_model->yt_fileNames_list();
    echo json_encode($data);
  }



  // Egy mp3 létezését és méretét ellenőrzi
  // A Youtube zenék ellenőrzéséhez használjuk
  function is_youtube_music_file($fileName)
  {
    $filePath = $this->config->config['yt_path'] . $fileName;
    if (file_exists($filePath) && filesize($filePath) > 1048576) {

      $data['result'] = "OK";
    } else {
      $data['result'] = "Hiányzik";
    }

    echo json_encode($data);
  }


  public function yt_music_check_file_exists($id)
  {
    $checkPath = $this->config->config['yt_path'];

    $data['yt_hiba'] = '';
    $SQLresult = $this->zenegepek_model->yt_music_check();
    $myLength = count($SQLresult);
    for ($i = 0; $i < $myLength; $i++) {
      $pathToFile = $this->config->config['yt_path'] . $SQLresult[$i]['mp3path'];
      if (!file_exists($pathToFile)) {
        $data['yt_hiba'] .= '<br>' . $i + 1 . '. ' . $SQLresult[$i]['artist'] . ' - ' . $SQLresult[$i]['title'] . '  - Nincs file';
      }
    }

    echo json_encode($data);
  }


  /* Az album könyvtárat ellenőrzi az alábbi szempontok szerint:
	- megvan-e a három alkönyvtár (audio, bitmap, text)
	- van-e ezen kívül további könyvtár vagy file az albumkönyvtárban
	- az audio könyvtárban mennyi mp3 file van
	- van-e más az audio alkönyvtárban
	- a bitmap könyvtárban léteznek-e a file-ok? (bitton.bmp, bitton.jpg, cover.bmp, cover.jpg)
	- megvan-e az md5 file ezekhez (illetve létrehoz md5 file-t)
	- a text könyvtárban megvan-e az album.dad, és hozzá az md5
	- van-e extra file a text könyvtárban

*/
  public function albumdir_check($albumkod)
  {
  }

  // Egy albumkönyvtár minde további bejegyzését (alkönyvtár, file) adatbázisba írja
  public function albumdir_to_database($albumkod)
  {
    $checkPath = $this->config->config['albums_path'];
    $albumdir = $checkPath . $albumkod;
    $data['albumkod'] = $albumkod;

    $myFiles = getDirContents($albumdir);

    foreach ($myFiles as $row) {
      $data['albumkod'] = $albumkod;
      $data['path'] = $row;
      $lastPerjel = strrpos($row, "/") - strlen($row) + 1;
      $fileName = substr($row, $lastPerjel);
      $data['filenev'] = $fileName;

      $result = $this->zenegepek_model->dir_check_insert($data);
    }

    $answer['result'] = $albumkod . ' - ready';
    echo json_encode($answer);
  }


  public function create_md5_file($albumkod)
  {
    $data['result'] = 'OK ';
    $checkPath = $this->config->config['media_path'];
    // BITTON
    $sourcePath = $checkPath . 'albums/' . $albumkod . '/bitmap/bitton.bmp';
    if (file_exists($sourcePath)) {
      $md5Content = md5_file($sourcePath) . '     bitton.bmp (' . $albumkod . ')';
      $md5File = $checkPath . 'albums/' . $albumkod . '/bitmap/bitton.md5';
      if (file_exists($md5File)) {
        unlink($md5File);
      }
      md5_file_write($md5File, $md5Content);
    } else {
      $data['result'] = $albumkod . ' - bitton.bmp not found';
    }
    // COVER
    $sourcePath = $checkPath . 'albums/' . $albumkod . '/bitmap/cover.bmp';
    if (file_exists($sourcePath)) {
      $md5Content = md5_file($sourcePath) . '     cover.bmp (' . $albumkod . ')';
      $md5File = $checkPath . 'albums/' . $albumkod . '/bitmap/cover.md5';
      if (file_exists($md5File)) {
        unlink($md5File);
      }
      md5_file_write($md5File, $md5Content);
    } else {
      $data['result'] .= $albumkod . ' - cover.bmp not found';
    }
    // DAD
    $sourcePath = $checkPath . 'albums/' . $albumkod . '/text/album.dad';
    if (file_exists($sourcePath)) {
      $md5Content = md5_file($sourcePath) . '     album.dad (' . $albumkod . ')';
      $md5File = $checkPath . 'albums/' . $albumkod . '/text/album.md5';
      if (file_exists($md5File)) {
        unlink($md5File);
      }
      md5_file_write($md5File, $md5Content);
    } else {
      $data['result'] .= $albumkod . ' - album.dad not found';
    }

    echo json_encode($data);
  }


  public function bmp_copy_back($albumkod)
  {
    $data['albumkod'] = $albumkod;
    $checkPath = $this->config->config['media_path'];

    // bitton.bmp
    $sourcePath = $checkPath . 'goodimages/bmp/bitton' . $albumkod . '.bmp';
    $data['bitton_bmp'] = 'bitton_bmp HIBA';
    if (file_exists($sourcePath)) {
      $targetFile = $checkPath . 'albums/' . $albumkod . '/bitmap/bitton.bmp';
      if (file_exists($targetFile)) {
        unlink($targetFile);
      }
      $copyResult = copy($sourcePath, $targetFile);
      if ($copyResult) {
        $data['bitton_bmp'] = 'bitton_bmp OK';
      }
    }
    // cover.bmp
    $sourcePath = $checkPath . 'goodimages/bmp/cover' . $albumkod . '.bmp';
    $data['cover_bmp'] = 'cover_bmp HIBA';
    if (file_exists($sourcePath)) {
      $targetFile = $checkPath . 'albums/' . $albumkod . '/bitmap/cover.bmp';
      if (file_exists($targetFile)) {
        unlink($targetFile);
      }
      $copyResult = copy($sourcePath, $targetFile);
      if ($copyResult) {
        $data['cover_bmp'] = 'cover_bmp OK';
      }
    }
    // bitton.jpg
    $sourcePath = $checkPath . 'goodimages/jpg/bitton' . $albumkod . '.jpg';
    $data['bitton_jpg'] = 'bitton_jpg HIBA';
    if (file_exists($sourcePath)) {
      $targetFile = $checkPath . 'albums/' . $albumkod . '/bitmap/bitton.jpg';
      if (file_exists($targetFile)) {
        unlink($targetFile);
      }
      $copyResult = copy($sourcePath, $targetFile);
      if ($copyResult) {
        $data['bitton_jpg'] = 'bitton_jpg OK';
      }
    }
    // cover.jpg
    $sourcePath = $checkPath . 'goodimages/jpg/cover' . $albumkod . '.jpg';
    $data['cover_jpg'] = 'cover_jpg HIBA';
    if (file_exists($sourcePath)) {
      $targetFile = $checkPath . 'albums/' . $albumkod . '/bitmap/cover.jpg';
      if (file_exists($targetFile)) {
        unlink($targetFile);
      }
      $copyResult = copy($sourcePath, $targetFile);
      if ($copyResult) {
        $data['cover_jpg'] = 'cover_jpg OK';
      }
    }

    echo json_encode($data);
  }

  public function app_user_add($deviceId)
  {
    $userIp = $_SERVER['REMOTE_ADDR'];
    $userAgent = $this->agent->platform;
    //if (str_contains($userAgent, 'indows')) { $data['desktop'] = true; } else { $data['desktop'] = false; }
    if (!isset($deviceId)) {
      $deviceId = '';
    }
    $registered = $this->mobil_model->yt_users_device_is_registered($deviceId);
    if (!$registered) // nincs nincs a rendszerben az device_id
    {
      $insertData['username'] = "";
      $insertData['phone'] = "";
      $insertData['email'] = "";
      $insertData['device_id'] = $deviceId;
      $insertData['opsystem'] = $userAgent;
      //  $insertData['dateinsert'] = date("Y-m-d");
      $insertData['ip'] = $userIp;
      $this->mobil_model->yt_users_insert($insertData);
    }

    $datarec = $this->mobil_model->yt_users_card_by_deviceid($deviceId);
    $data['username'] = $datarec['username'];
    $data['device_id'] = $datarec['device_id'];
    $data['jukeboxes'] = count($this->mobil_model->yt_user_jukeboxes($deviceId));
    $data['submits'] = count($this->mobil_model->submits_by_deviceid($deviceId));

    echo json_encode($data);
  }

  public function app_user_jukeboxes($deviceId)
  {
    $userIp = $_SERVER['REMOTE_ADDR'];
    $userAgent = $this->agent->platform;

    $datarec = $this->mobil_model->yt_users_card_by_deviceid($deviceId);
    $data['username'] = $datarec['username'];
    $data['device_id'] = $datarec['device_id'];
    $data['jukeboxes'] = $this->mobil_model->yt_user_jukeboxes($deviceId);

    echo json_encode($data);
  }

  // Képernyő szélesség lekérdezés
  public function getscreenwidth()
  {
    $data = '';
    $this->load->view('getscreenwidth', $data);
  }

  // Hangerő állítás
  public function setvolume($servcode, $aktVolume, $secretCode)
  {
    if ($secretCode == "A1D2C4E5") {
      $data['servcode'] = $servcode;
      $data['aktvolume'] = $aktVolume * 5;
      $result = $this->zenegepek_model->soundsettings_update($data);
    }
    //	Nem kell választ küldeni
  }
}
