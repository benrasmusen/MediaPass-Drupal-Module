<?php
function mediapass_metered_settings($form, &$form_state) {
	
	if (empty($form_state['input'])) {
		$response = mediapass_api_request('metered', array(
			'method' => 'GET',
			'params' => array(
				'Id' => variable_get('MP_user_ID')
			)
		));
	} else {
		$response = $form_state['input'];
	}
	
	$values = array_merge(array(
		'Status' => null,
		'Count' => null
	), (array) $response);
	
	$form['container'] = array(
		'#type' => 'fieldset',
		'#title' => t('Metered Settings'),
		'#description' => t('Update your MediaPass Metered Settings.')
	);
	$form['container']['Status'] = array(
		'#type' => 'select',
		'#title' => t('Status'),
		'#value' => $values['Status'],
		'#options' => array(
			'Off' => 'Off',
			'On' => 'On'
		)
	);
	$form['container']['Count'] = array(
		'#type' => 'textfield',
		'#title' => t('Count'),
		'#value' => $values['Count']
	);
	
	
	$form['container']['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Update Metered Settings')
	);
	$form['#submit'][] = 'medipass_metered_submit';
	
	return $form;
}

function medipass_metered_submit($form, &$form_state) {
	
	$data = $form_state['input'];
	
	$fields = array(
		'Status',
		'Count'
	);
	
	foreach ($data as $key => $value) {
		if (!in_array($key, $fields)) {
			unset($data[$key]);
		}
	}
	
	$response = mediapass_api_request('metered', array(
		'method' => 'POST',
		'params' => array(
			'Id' => variable_get('MP_user_ID')
		),
		'data' => $data
	));
	
	if ($response) {
		drupal_set_message(t('Updated successfully!'));
	}
	
}