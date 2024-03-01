<?php
defined('BASEPATH') or exit('No direct script access allowed');
?>

<!DOCTYPE html>
<html lang="hu" class=" ">

<head>
  <!-- 
      * @Package: Detonator Mobile App
      * @Author: Linutek
      * @Version: 2.5-app
      -->
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1.0" />
  <title><?php if (isset($title)) {
            echo $title;
          } ?> </title>
  <meta content="Detonator Mobile App" name="description" />
  <meta content="Linutek" name="author" />

  <link rel="icon" type="image/png" sizes="32x32" href="<?php echo base_url(); ?>assets/images/icons/favicon-32x32.png">
  <link rel="icon" type="image/png" sizes="16x16" href="<?php echo base_url(); ?>assets/images/icons/favicon-16x16.png">
  <script src="<?php echo $base_url .  $this->config->config['pwa_index'];; ?>" defer></script>
  <link rel="manifest" href="<?php echo $base_url . $this->config->config['pwa_manifest']; ?>">
  <meta name="msapplication-TileColor" content="#ffffff">
  <meta name="msapplication-TileImage" content="<?php echo base_url(); ?>assets/images/icons/ms-icon-150x150.png">
  <meta name="theme-color" content="#ffffff">

  <?php if (isset($needSlider)) {
    if ($needSlider === 'YES') {   ?>
      <?php // OTHER SCRIPTS INCLUDED ON THIS PAGE - START 
      ?>
      <script src="<?php echo base_url(); ?>assets/js/nouislider.js" type="text/javascript"></script>
      <script src="<?php echo base_url(); ?>assets/js/wNumb.min.js" type="text/javascript"></script>
      <link href="<?php echo base_url(); ?>assets/css/det-nouislider.css" rel="stylesheet" type="text/css" media="screen" />
      <link href="<?php echo base_url(); ?>assets/css/nouislider.css" rel="stylesheet" type="text/css" media="screen" />
      <?php //  OTHER SCRIPTS INCLUDED ON THIS PAGE - END 
      ?>
  <?php }
  }  ?>


  <?php // CORE CSS TEMPLATE - START 
  ?>
  <?php $timenow = gettimeofday(); ?>
  <link href="<?php echo base_url() . 'assets/css/det-style.css?' . $timenow['usec']; ?>" type="text/css" rel="stylesheet" media="screen,projection" id="main-style" />
  <?php // CORE CSS TEMPLATE - END 
  ?>

  <?php loadHeaderCss(); ?>

  <script src="<?php echo $base_url . 'assets/js/device-uuid.min.js'; ?>"></script>

</head>

<?php //  BEGIN BODY 
?>

<body class="app-body">