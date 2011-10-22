<?php

function mediapass_account_info($form, &$form_state) {
	
	if (empty($form_state['input'])) {
		$response = mediapass_api_request('account', array(
			'method' => 'GET',
			'params' => array(
				'Id' => variable_get('MP_user_ID')
			)
		));
	} else {
		$response = $form_state['input'];
	}
	
	$values = array_merge(array(
		'Title' => null,
	    'CompanyName' => null,
	    'CompanyAddress' => null,
	    'Country' => null,
	    'City' => null,
	    'State' => null,
	    'Zip' => null,
	    'Telephone' => null,
	    'CustomRedirectURL' => null,
	    'CustomSalePostURL' => null
	), (array) $response);
	
	$form['container'] = array(
		'#type' => 'fieldset',
		'#title' => t('MediaPass Account Info'),
		'#description' => t('Manage your MediaPass account information.')
	);
	$form['container']['Title'] = array(
		'#type' => 'textfield',
		'#title' => t('Web Site Subject/Title'),
		'#value' => $values['Title']
	);
	$form['container']['CompanyName'] = array(
		'#type' => 'textfield',
		'#title' => t('Company Name'),
		'#value' => $values['CompanyName']
	);
	$form['container']['CompanyAddress'] = array(
		'#type' => 'textfield',
		'#title' => t('Company Address'),
		'#value' => $values['CompanyAddress']
	);
	$form['container']['City'] = array(
		'#type' => 'textfield',
		'#title' => t('City'),
		'#value' => $values['City']
	);
	$form['container']['State'] = array(
		'#type' => 'textfield',
		'#title' => t('State'),
		'#value' => $values['State']
	);
	$form['container']['Zip'] = array(
		'#type' => 'textfield',
		'#title' => t('ZIP or Postal Code'),
		'#value' => $values['Zip']
	);
	$form['container']['Country'] = array(
		'#type' => 'textfield',
		'#title' => t('Country'),
		'#value' => $values['Country']
	);
	$form['container']['Telephone'] = array(
		'#type' => 'textfield',
		'#title' => t('Telephone'),
		'#value' => $values['Telephone']
	);
	$form['container']['CustomRedirectURL'] = array(
		'#type' => 'textfield',
		'#title' => t('Custom Redirect URL (http://)'),
		'#value' => $values['CustomRedirectURL']
	);
	$form['container']['CustomSalePostURL'] = array(
		'#type' => 'textfield',
		'#title' => t('Custom Sale Posting URL'),
		'#value' => $values['CustomSalePostURL']
	);
	$form['container']['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Update Account')
	);
	$form['#submit'][] = 'medipass_account_info_submit';
	
	return $form;
}

function medipass_account_info_submit($form, &$form_state) {
	
	$data = $form_state['input'];
	
	$fields = array(
		'Id',
		'Title',
	    'CompanyName',
	    'CompanyAddress',
	    'Country',
	    'City',
	    'State',
	    'Zip',
	    'Telephone',
	    'CustomRedirectURL',
	    'CustomSalePostURL'
	);
	
	foreach ($data as $key => $value) {
		if (!in_array($key, $fields)) {
			unset($data[$key]);
		}
	}
	
	$response = mediapass_api_request('account', array(
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