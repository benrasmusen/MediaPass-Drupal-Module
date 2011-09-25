<?php
function mediapass_ecpm_floor($form, &$form_state) {
	
	_checkSetup();
	
	if (empty($form_state['input'])) {
		$response = mediapass_api_request('ecpm', array(
			'method' => 'GET',
			'params' => array(
				'Id' => variable_get('MP_user_ID')
			)
		));
	} else {
		$response = $form_state['input'];
	}
	
	$values = array_merge(array(
		'ecpm_floor' => null
	), (array) $response);
	
	$form['container'] = array(
		'#type' => 'fieldset',
		'#title' => t('MediaPass eCPM Floor'),
		'#description' => t('Update your MediaPass eCPM Floor.')
	);
	$form['container']['ecpm_floor'] = array(
		'#type' => 'textfield',
		'#title' => t('eCPM Floor'),
		'#value' => $values['ecpm_floor']
	);
	
	
	$form['container']['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Update eCPM Floor')
	);
	$form['#submit'][] = 'medipass_ecpm_floor_submit';
	
	return $form;
	
}

function medipass_ecpm_floor_submit($form, &$form_state) {
	
	$data = $form_state['values'];
	
	$fields = array(
		'ecpm_floor'
	);
	
	foreach ($data as $key => $value) {
		if (!in_array($key, $fields)) {
			unset($data[$key]);
		}
	}
	
	$response = mediapass_api_request('ecpm', array(
		'method' => 'POST',
		'params' => array(
			variable_get('MP_user_ID'),
			$data['ecpm_floor']
		)
	));
	
	if ($response) {
		drupal_set_message(t('Updated successfully!'));
	}
	
}