<?php
defined('BASEPATH') or exit('No direct script access allowed');
?>

<div class="admin-container fp">

  <h3><?php echo $pageheading; ?></h3>

  <form class="padding-5px" action="<?php echo base_url() . 'index.php/helyek_list/0'; ?>" method="post">
    <div class="input-group">
      <div class="form-outline">
        <input type="search" name="kereses" size="25" id="form1" class="form-control" />
      </div>
      <button type="submit" name="btn_keres" value="keres" class="btn btn-primary"><i class="bi-search"></i></button>
    </div>
  </form>

  <div class="det-spacer"></div>

  <a class="btn btn-warning btn-margin" href="<?php echo base_url() . 'index.php/helyek_edit/0'; ?>" role="button">Hely hozzáadása</a>
  <div class="det-spacer"></div>

  <table class="table table-striped">
    <thead>
      <tr>
        <th>Vendéglátóhely&nbsp;</th>
        <th>Város&nbsp;</th>
        <th class="for-desktop">Kapcsolattartó&nbsp;</th>
        <th class="for-desktop">Telefon&nbsp;</th>
      </tr>
    </thead>
    <tbody>

      <?php
      foreach ($query as $sor) {    ?>
        <tr>
          <?php $helyneve = $sor['nev'];
          if (strlen($helyneve) < 2) {
            $helyneve = 'Nincs megadva';
          }
          ?>
          <td><?php echo anchor($uri = base_url() . 'index.php/helyek_edit/' . $sor['id'], $title = $helyneve, $attributes = ''); ?></td>
          <td><?php echo  $sor['varos'] . ' - ' . $sor['utca']; ?></td>
          <td class="for-desktop"><?php echo $sor['kapcsolattarto_nev']; ?></td>
          <td class="for-desktop"><?php echo $sor['telefon']; ?></td>
        </tr>
      <?php } ?>

    </tbody>
  </table>

  <?php show_pagination($links); ?>

</div> <!--- container -->