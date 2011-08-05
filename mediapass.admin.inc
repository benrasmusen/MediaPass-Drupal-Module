<?php
/**
* Form function, called by drupal_get_form()
* in mediapass_menu().
*/
function mediapass_form($form, &$form_state) {
	
	$form['mediapass_max'] = array(
		'#type' => 'textfield',
		'#title' => t('Maximum number of posts'),
		'#default_value' => variable_get('current_posts_max', 3),
		'#size' => 2,
		'#maxlength' => 2,
		'#description' => t('The maximum numbers of links to display in the block.'),
		'#required' => TRUE
	);

	return system_settings_form($form);
	
}


function mediapass_contents() {
	
	//Get today's date.
	$today = getdate();
	//Calculate midnight a week ago.
	$start_time = mktime(0, 0, 0,$today['mon'],($today['mday'] - 7), $today['year']);
	//Get all posts from one week ago to the present.
	$end_time = time();

	//NEW LINE
	$max_num = variable_get('current_posts_max', 3);

	//Use Database API to retrieve current posts.
	$query = db_select('node', 'n')
		->fields('n', array('nid', 'title', 'created'))
		->condition('status', 1) //Published.
		->condition('created', array($start_time, $end_time), 'BETWEEN')
		->orderBy('created', 'DESC') //Most recent first.
		->range(0, $max_num) //NEW LINE
		->execute();
		
	return $query;
	
}
