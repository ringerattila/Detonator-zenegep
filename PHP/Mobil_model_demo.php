<?php if (!defined('BASEPATH')) exit('Közvetlen elérés letiltva!');

// class MObil_model extends CI_Model {
class Mobil_Model extends CI_Model
{


  public function __construct()
  {
    parent::__construct();
  }


  // Az SQLrec tartalmától függően kiegészíti a query stringet
  // $strsql - bemenet, az eddig összeállított SQL string, ehhez
  // teszi hozzá a WHERE, ORDRBY, PAGE, stb. stringeket
  private function where_string($strsql, $SQL_pars)
  {
    $str_keres = $SQL_pars['str_keres'];
    $search_fields = $SQL_pars['search_fields'];
    $having = $SQL_pars['having'];
    $orderby = $SQL_pars['orderby'];
    if (isset($SQL_pars['groupby'])) {
      $groupby = $SQL_pars['groupby'];
    }
    $ordertype = $SQL_pars['ordertype'];
    $page = $SQL_pars['page'];
    $per_page = $SQL_pars['per_page'];

    if ($str_keres != "") {
      //$str_keres = $wherestr;
      $sfields_num = count($search_fields);  // ennyi elem van a tömbben
      $sfields_mut = 0;
      $whstr = ' (';  // az elején nyitó zárójel

      foreach ($search_fields as $sfield) {
        $whstr .=  $sfield . ' LIKE "%' . $str_keres . '%" ';
        $sfields_mut++;  // elem mutató növelése
        if ($sfields_mut != $sfields_num) {
          $whstr .= ' OR ';
        }
      } // foreach   

      $strsql .= " WHERE " . $whstr . ')';
    }  // wherestr vége

    if ($having != "") {
      $strsql .= " HAVING " . $having;
    }

    if (isset($groupby)) {
      $strsql .= " GROUP BY " . $groupby;
    }


    if ($orderby != "") {
      $strsql .= " ORDER BY " . $orderby . " " . $ordertype;
    }

    if (!is_null($page)) {
      $strsql .= " LIMIT " . $page . ', ' . $per_page;
    }

    return $strsql; // ezzel térünk vissza
  } // end of function


  function yt_users_lista($SQLrec)
  {
    $SQL_pars = array(
      'str_keres' => $SQLrec['str_keres'],
      'search_fields' => array('phone', 'username', 'email', 'yu.device_id'),   // ezekben a mezőkben keres
      'geptipus' => '',
      'having' => '',
      'orderby' => '',
      'ordertype' => '',
      'page' => NULL,
      'per_page' => NULL
    );

    // AZ SQL string első része ugyanaz mindekét esetben
    $SQL_str = 'SELECT  '
      . ' yu.id as id, '
      . ' yu.phone as phone, '
      . ' yu.username as username, '
      . ' yu.email as email, '
      . ' yu.device_id as device_id, '
      . ' yu.opsystem as opsystem, '
      . ' COUNT(DISTINCT ys.id) AS submit_count, '
      . ' COUNT(DISTINCT yug.id) AS gep_count '
      . ' FROM yt_users yu '
      // uj rész
      . ' LEFT JOIN '
      . ' yt_submits ys ON yu.device_id = ys.device_id '
      . ' LEFT JOIN '
      . ' yt_user_gep yug ON yu.device_id = yug.device_id ';
    //   . ' GROUP BY  yu.device_id ';


    if ($SQLrec['query_type'] === 'total_rows') {
      //A rekordok számát állapítjuk meg, kereséssel együtt
      $strsql = $this->where_string($SQL_str, $SQL_pars);
      $query = $this->db->query($strsql);
      //$query->num_rows();
      $result['total_rows'] = $query->num_rows(); // a rekordok száma
      return $result;
    } else {  // SQL lekérés
      // paraméterek kiegészítése   
      $SQL_pars['geptipus'] = $SQLrec['type'];
      $SQL_pars['orderby'] = $SQLrec['orderby'];
      $SQL_pars['ordertype'] = $SQLrec['ordertype'];
      $SQL_pars['page'] = $SQLrec['page'];
      $SQL_pars['per_page'] = $SQLrec['per_page'];

      $strsql = $this->where_string($SQL_str, $SQL_pars);
      $query = $this->db->query($strsql);
      $result['query'] = $query->result_array(); // a lekérés eredménytöbmje
      return $result;
    }
  } // end of function




  function admin_yt_users_lista($SQLrec)
  {
    // AZ SQL string első része ugyanaz mindekét esetben
    $SQL_str = 'SELECT  '
      . ' yu.id as id, '
      . ' yu.phone as phone, '
      . ' yu.username as username, '
      . ' yu.email as email, '
      . ' yu.device_id as device_id, '
      . ' yu.opsystem as opsystem, '
      . ' COUNT(DISTINCT ys.id) AS submit_count, '
      . ' COUNT(DISTINCT yug.id) AS gep_count '
      . ' FROM yt_users yu '
      // uj rész
      . ' LEFT JOIN '
      . ' yt_submits ys ON yu.device_id = ys.device_id '
      . ' LEFT JOIN '
      . ' yt_user_gep yug ON yu.device_id = yug.device_id ';

    // ha nem ürse a keresőszó, akkor szűkítünk
    if (isset($SQLrec['str_keres']) && $SQLrec['str_keres'] != "") {
      $SQL_str .= ' WHERE (yu.phone LIKE "%' . $SQLrec['str_keres'] . '%" '
        . ' OR yu.username LIKE "%' . $SQLrec['str_keres'] . '%" '
        . ' OR yu.email LIKE "%' . $SQLrec['str_keres'] . '%" '
        . ' OR yu.device_id LIKE "%' . $SQLrec['str_keres'] . '%" '
        . ' OR yu.opsystem LIKE "%' . $SQLrec['str_keres'] . '%" ) ';
    }

    $SQL_str .= ' GROUP BY  yu.device_id '
      . ' ORDER BY submit_count DESC ';

    $query = $this->db->query($SQL_str);
    $result['query'] = $query->result_array(); // a lekérés eredménytöbmje
    return $result;
  } // end of function


  public function yt_users_insert($data)
  {
    // Query to insert data in database
    $this->db->insert('yt_users', $data);
    return true;
  }

  // Meglévő adatrekord módosítása
  public function yt_users_update($data)
  {
    $id = $data['id'];
    $this->db->where('id', $id);
    $this->db->update('yt_users', $data);
  }

  public function yt_users_delete($id)
  {
    $this->db->query('DELETE FROM yt_users WHERE id = "' . $id . '"');
    return true;
  }

  // 
  public function yt_users_device_is_registered($deviceId)
  {
    $condition = "device_id =" . "'" . $deviceId . "'";
    $this->db->select('*');
    $this->db->from('yt_users');
    $this->db->where($condition);
    $this->db->limit(1);
    $query = $this->db->get();

    if ($query->num_rows() == 1) {
      return true;
    } else {
      return false;
    }
  } // 

  // Beküldő karton ID alapján
  public function yt_users_card_by_id($id)
  {
    $condition = "id =" . "'" . $id . "'";
    $this->db->select('*');
    $this->db->from('yt_users');
    $this->db->where($condition);
    $this->db->limit(1);
    $query = $this->db->get();

    if ($query->num_rows() == 1) {
      return $query->result_array();
    } else {
      return false;
    }
  } // end yt_users_card_by_id()


  // Beküldő karton device ID alapján
  public function yt_users_card_by_deviceid($device_id)
  {
    $condition = "device_id =" . "'" . $device_id . "'";
    $this->db->select('*');
    $this->db->from('yt_users');
    $this->db->where($condition);
    $this->db->limit(1);
    $query = $this->db->get();

    if ($query->num_rows() == 1) {
      return $query->row_array();
    } else {
      return false;
    }
  } // end yt_users_card_by_deviceid()


  // Meglévő adatrekord módosítása
  public function yt_username_update($data)
  {
    $device_id = $data['device_id'];
    $this->db->where('device_id', $device_id);
    $this->db->update('yt_users', $data);
  }


  // Device_id alapján törli a felhasználót.
  public function yt_delete_me($device_id)
  {
    // Query to insert data in database
    //    $this->db->insert('albumlistak', $data);
    $this->db->query('DELETE FROM yt_users WHERE device_id = "' . $device_id . '"');
    return true;
  }

  // Felhasználó - zenegép összerendelések
  // Az adott felhasználóhoz melyik zenegépek vannak hozzárendelve
  function yt_user_gep_total_rows($str_keres)
  {
    if ($str_keres === '') {  // nincs keresés, mindent listáz
      $query = $this->db->query('SELECT * FROM yt_user_gep');
    } else { // keresés miatti szűkítés

      $query = $this->db->query('SELECT  '
        . ' ug.id id, '
        . ' ug.servcode servcode, '
        . ' gk.gyarszam  gyarszam, '
        . ' ug.gepnev gepnev, '
        . ' us.username username '
        . ' left outer join gepkartya gk on gk.servcode = ug.servcode  '
        . ' left outer join yt_users us on ug.user_id = us.id '
        . ' FROM yt_user_gep ug '
        . ' WHERE (servcode LIKE "%' . $str_keres . '%")'
        . ' OR (username LIKE "%' . $str_keres . '%") '
        . ' OR (gyarszam LIKE "%' . $str_keres . '%") ');
    }

    $query->result_array();
    return $query->num_rows();
  }

  function yt_user_gep_lista($SQLrec)
  {
    $SQL_pars = array(
      'str_keres' => $SQLrec['str_keres'],
      'search_fields' => array('ug.servcode', 'gc.gepneve', 'yu.username', 'ug.device_id'),   // ezekben a mezőkben keres
      'geptipus' => '',
      'having' => '',
      'orderby' => '',
      'ordertype' => '',
      'page' => NULL,
      'per_page' => NULL
    );

    // AZ SQL string első része ugyanaz mindkét esetben
    $SQL_str = 'SELECT  '
      . ' ug.id id, '
      . ' ug.device_id device_id, '
      . ' ug.servcode servcode, '
      . ' ug.canadmin  canadmin, '
      . ' gc.gepneve gepneve, '
      . ' yu.username username '
      . ' FROM yt_user_gep ug '
      . ' left outer join detnet_gepconnect gc on gc.servcode = ug.servcode  '
      . ' left outer join yt_users yu on ug.device_id = yu.device_id ';

    if ($SQLrec['query_type'] === 'total_rows') {
      //A rekordok számát állapítjuk meg, kereséssel együtt
      $strsql = $this->where_string($SQL_str, $SQL_pars);
      $query = $this->db->query($strsql);
      //$query->num_rows();
      $result['total_rows'] = $query->num_rows(); // a rekordok száma
      return $result;
    } else {  // SQL lekérés
      // paraméterek kiegészítése   
      $SQL_pars['geptipus'] = $SQLrec['type'];
      $SQL_pars['orderby'] = $SQLrec['orderby'];
      $SQL_pars['ordertype'] = $SQLrec['ordertype'];
      $SQL_pars['page'] = $SQLrec['page'];
      $SQL_pars['per_page'] = $SQLrec['per_page'];

      $strsql = $this->where_string($SQL_str, $SQL_pars);
      $query = $this->db->query($strsql);
      $result['query'] = $query->result_array(); // a lekérés eredménytöbmje
      return $result;
    }
  }

  // Adott Beküldőhöz rendelt zenegépek listája
  function admin_yt_user_gep_lista($SQLrec)
  {
    // AZ SQL string első része ugyanaz mindkét esetben
    $SQL_str = 'SELECT  '
      . ' ug.id id, '
      . ' ug.device_id device_id, '
      . ' ug.servcode servcode, '
      . ' ug.canadmin  canadmin, '
      . ' vc.nickname  nickname, '
      . ' vc.gyarszam  gyarszam, '
      . ' vc.enabled enabled, '
      . ' yu.username username '
      . ' FROM yt_user_gep ug '
      //        . ' LEFT JOIN detnet_gepconnect gc on gc.servcode = ug.servcode  '
      . ' LEFT JOIN vi_gepconnect vc on vc.servcode = ug.servcode  '
      . ' LEFT JOIN yt_users yu on ug.device_id = yu.device_id ';


    // ha nem ürse a keresőszó, akkor szűkítünk
    if (isset($SQLrec['str_keres']) && $SQLrec['str_keres'] != "") {
      $SQL_str .= ' WHERE (ug.device_id = "' . $SQLrec['str_keres'] . '") ';
    }

    $SQL_str .= ' ORDER BY ug.servcode ASC ';

    $query = $this->db->query($SQL_str);
    $result['query'] = $query->result_array(); // a lekérés eredménytöbmje
    return $result;
  }


  public function submits_by_servcode($device_id)
  {
    $strsql = 'SELECT yt.servcode as servcode, count(yt.servcode) as calls, gc.gepneve as gepneve'
      . ' FROM yt_submits yt '
      . ' LEFT OUTER JOIN detnet_gepconnect gc on yt.servcode=gc.servcode '
      . 'WHERE device_id="'
      . $device_id . '" GROUP BY servcode ORDER BY calls DESC LIMIT 10';

    $query = $this->db->query($strsql);
    return $query->result_array();
  }

  public function submits_by_deviceid($deviceId)
  {
    $strsql = 'SELECT yt.servcode as servcode, gk.hely_id as hely_id, fk.nev as helyneve, yt.yt_id, '
      .  ' date_format(yt.bekuldve, "%Y-%m-%d") as bekuldve, ym.videoid as videoid, ym.artist, ym.title '
      . ' FROM yt_submits yt '
      . ' LEFT OUTER JOIN gepkartya gk ON yt.servcode = gk.servcode '
      . ' LEFT OUTER JOIN felallitasi_hely fk ON fk.id = gk.hely_id '
      . ' LEFT OUTER JOIN yt_music ym on ym.videoid = yt.yt_id '
      . ' WHERE device_id="'
      . $deviceId . '" ORDER BY yt.bekuldve DESC LIMIT 50';

    $query = $this->db->query($strsql);
    return $query->result_array();
  }

  // egy adott Beküldő zene beküldéseinek száma
  function yt_user_submits_total_rows($device_id)
  {
    $strsql = "SELECT * "
      . " FROM yt_submits yt "
      . ' WHERE device_id="' . $device_id . '" LIMIT 20';

    $query = $this->db->query($strsql);
    return $query->num_rows();
  }

  // egy adott Beküldő zene beküldései
  public function yt_user_submits($device_id)
  {
    $strsql = "SELECT date_format(yt.bekuldve, '%Y-%m-%d') as bekuldve, ym.videoid, ym.artist, ym.title "
      . " FROM yt_submits yt "
      . ' LEFT OUTER JOIN yt_music ym on ym.videoid=yt.yt_id '
      . ' WHERE device_id="' . $device_id . '" ORDER BY yt.bekuldve DESC LIMIT 20';

    $query = $this->db->query($strsql);
    return $query->result_array();
  }

  // Megnézi, hogy egy adott zeneszám egy adott zenegép youtube listáján
  // rajta van-e
  public function yt_submits_check_onlinst($data)
  {
    $strsql = 'SELECT yt_id as onlist '
      . ' FROM yt_submits WHERE yt_id="'
      . $data['yt_id'] . '" AND servcode="' . $data['servcode'] . '"';

    $query = $this->db->query($strsql);
    return $query->num_rows();
  } // end of yt_submits_check_onlist

  public function yt_submits_insert($data)
  {
    // Query to insert data in database
    $this->db->insert('yt_submits', $data);
    $servcode = $data['servcode'];
    $data = array(
      'servcode' => $servcode,
      'playlistmod' => 1,
    );
    $this->db->where('servcode', $servcode);
    $this->db->update('detnet_realtime', $data);
    return true;
  }

  // Meglévő adatrekord módosítása
  public function yt_submits_update($data)
  {
    $id = $data['id'];
    $this->db->where('id', $id);
    $this->db->update('yt_submits', $data);
  }

  // Meglévő adatrekord módosítása
  public function yt_submits_delete($id)
  {
    $this->db->where('id', $id);
    $this->db->delete('yt_submits');
  }

  // Adott beküldő beküldéseit töröljk
  public function yt_delete_user_submits($device_id)
  {
    $this->db->where('device_id', $device_id);
    $this->db->delete('yt_submits');
  }

  // Delete records by videoid
  public function yt_submits_delete_by_videoid($videoId)
  {
    $this->db->where('yt_id', $videoId);
    $this->db->delete('yt_submits');
  }

  public function yt_submits_update_datetime($data)
  {
    $strsql = 'UPDATE yt_submits SET bekuldve=NOW()'
      . ' WHERE yt_id="' . $data['yt_id'] . '" '
      . 'AND servcode="' . $data['servcode'] . '"';

    $query = $this->db->query($strsql);
    return $query;
  }

  // megnézi, melyek azok  a zenék, amik már nem léteznek a youtube zenék között
  public function dead_submits()
  {
    $strsql = 'SELECT ys.*  FROM yt_submits ys
    LEFT JOIN yt_music ym ON ys.yt_id = ym.videoid
    WHERE ym.videoid IS NULL
    ORDER BY ys.bekuldve DESC';

    $query = $this->db->query($strsql);
    return $query->result_array();
  }


  // Megnézi, hogy egy adott zenegép engedélyezett-e
  public function yt_zenegep_check_enabled($servcode)
  {
    $strsql = 'SELECT nickname '
      . ' FROM gepkartya WHERE servcode="'
      . $servcode . '" AND enabled="Y"';

    $query = $this->db->query($strsql);
    return $query->num_rows();
  } // end of yt_zenegep_check_enabled

  // adott azonosítújú felhasználó kártyája
  function yt_user_gep_card($id)
  {
    $strsql = "SELECT * FROM yt_user_gep WHERE id = " . $id;

    $query = $this->db->query($strsql);
    return $query->result_array();
  }

  // Megnézi, hogy egy adott zenegép hozzá van-e rendelve a felhasználóhoz
  public function yt_check_user_gep($data)
  {
    $strsql = 'SELECT * '
      . ' FROM yt_user_gep WHERE device_id="'
      . $data['device_id'] . '" AND servcode="' . $data['servcode'] . '"';

    $query = $this->db->query($strsql);
    return $query->num_rows();
  } // end of yt_submits_check_onlist

  public function yt_user_gep_insert($data)
  {
    if (strlen($data['servcode']) > 8) { // Túl hosszú szervizkód
      $message = 'Rossz azonosító.';
      return $message;
    }
    if (strlen($data['servcode']) < 8) {   // Túl rövid szervizkód
      $message = 'Rossz azonosító.';
      return $message;
    }
    if (!is_numeric($data['servcode'])) {   // Nem csak számokól áll
      $message = 'Rossz azonosító.';
      return $message;
    }
    $gep_enabled = $this->mobil_model->yt_zenegep_check_enabled($data['servcode']);
    if ($gep_enabled === 0) {   // Létezik-e ilyen aktív zenegép
      $message = 'Nincs ilyen aktív zenegép.';
      return $message;
    }
    $assigned = $this->mobil_model->yt_check_user_gep($data);
    if ($assigned != 0) { // Már az emberhez van regisztrálva
      $message = 'Ez a zenegép már hozzád van regisztrálva.';
      return $message;
    }
    // Nincs semmi hiba, beszúrhatjuk
    $this->db->insert('yt_user_gep', $data);
    $message = 'Sikeres zenegép regisztráció.';
    return $message;
  }

  // Meglévő adatrekord módosítása
  public function yt_user_gep_update($data)
  {
    $id = $data['id'];
    $this->db->where('id', $id);
    $this->db->update('yt_user_gep', $data);
  }

  public function yt_user_gep_delete($id)
  {
    // Query to insert data in database
    //    $this->db->insert('albumlistak', $data);
    $this->db->query('DELETE FROM yt_user_gep WHERE id = "' . $id . '"');
    return true;
  }

  // egy adott Beküldőhöz rendelt zenegépek száma
  // csak az engedélyezett gépek jelennek meg
  function yt_user_jukeboxes($deviceId)
  {
    $strsql = "SELECT ug.servcode as servcode, gk.hely_id as hely_id, fh.nev as helyneve, "
      . " ug.canadmin canadmin "
      . " FROM yt_user_gep ug "
      . ' LEFT OUTER JOIN gepkartya gk ON gk.servcode = ug.servcode '
      . ' LEFT OUTER JOIN felallitasi_hely fh ON fh.id = gk.hely_id '
      . ' WHERE ug.device_id="' . $deviceId . '"';

    $query = $this->db->query($strsql);
    return $query->result_array();;
  }

  // egy adott Beküldőhöz rendelt zenegépek száma
  function yt_user_jukeboxes_total_rows($device_id)
  {
    $strsql = "SELECT * "
      . " FROM yt_user_gep "
      . ' WHERE device_id="' . $device_id . '" LIMIT 20';

    $query = $this->db->query($strsql);
    return $query->num_rows();
  }

  // Device_id alapján törli a felhasználót.
  public function yt_delete_user_jukeboxes($device_id)
  {
    // Query to insert data in database
    //    $this->db->insert('albumlistak', $data);
    $this->db->query('DELETE FROM yt_user_gep WHERE device_id = "' . $device_id . '"');
    return true;
  }


  // adott szervizkódú zenegép adatlapja
  function yt_jukebox_card($servcode)
  {
    $strsql = 'SELECT dc.creditsnow as creditsnow, dc.shortcounters as shortcounters, dc.servcode as servcode, gc.status as status, '
      . ' dso.aktvolume as aktvolume, dset.freemodetype as freemodetype, gk.gyarszam as gyarszam '
      . ' FROM detnet_counters dc '
      . ' LEFT OUTER JOIN detnet_gepconnect gc ON gc.servcode = dc.servcode '
      . ' LEFT OUTER JOIN gepkartya gk ON gk.servcode = dc.servcode '
      . ' LEFT OUTER JOIN detnet_soundsettings dso ON dso.servcode = dc.servcode '
      . ' LEFT OUTER JOIN detnet_settings dset ON dset.servcode = dc.servcode '
      . ' WHERE dc.servcode = "' . $servcode . '"';

    $query = $this->db->query($strsql);
    return $query->result_array();
  }


  /* End of file mobil_model.php */
}
