<?php

function mediapass_link() {
	
	$mp_user_ID = variable_get('MP_user_ID');
	$mp_access_token = variable_get('MP_access_token');
	$mp_refresh_token = variable_get('MP_refresh_token');
	
	if (!$mp_user_ID && !$mp_refresh_token && !$mp_access_token) {
	
	$register_url = MP_AUTH_REGISTER_URL . urlencode("http" . ((!empty($_SERVER['HTTPS']) && $_SERVER['HTTPS'] == 'on') ? "s" : null) . "://" . $_SERVER['SERVER_NAME'].$_SERVER['REQUEST_URI']);
	
	$login_url = MP_AUTH_LOGIN_URL . urlencode("http" . ((!empty($_SERVER['HTTPS']) && $_SERVER['HTTPS'] == 'on') ? "s" : null) . "://" . $_SERVER['SERVER_NAME'].$_SERVER['REQUEST_URI']);
	
$html = <<<STRING
<div class="mp-wrap">

	<h2 class="header">Signup</h2>

	<h3>Create an account in just a few minutes...</h3>

	<p>No credit card required. No technical expertise needed. Go live in just a few minutes! <a href="{$register_url}">Click here to get started!</a></p>

	<hr/>

	<h3>Already have an account?</h3>

	<p><a href="{$login_url}">Authorize your account now!</a></p>

</div>
STRING;

	} else {
		
		!empty($_GET['access_token']) && !empty($_GET['refresh_token']) && !empty($_GET['id']) {
			drupal_set_message(t('Successfully linked MediaPass account.'), 'status');
		}
		
$html = <<<STRING
<div class="mp-wrap">

	<h2 class="header">Unlink Account</h2>
	
	<a href="{$_SERVER['REQUEST_URI']}?deauth=true">Click here to unlink your MediaPass Account.</a>

</div>
STRING;
		
	}

	return $html;
	
}