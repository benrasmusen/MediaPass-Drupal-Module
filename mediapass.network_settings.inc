<?php
function mediapass_network_settings($form, &$form_state) {
	
	_checkSetup();
	
	$response = mediapass_api_request('network/list', array(
		'method' => 'GET',
		'params' => array(
			'Id' => variable_get('MP_user_ID')
		)
	));
	
	$form['existing_container'] = array(
		'#type' => 'fieldset',
		'#title' => t('Existing Sites')
	);
	
	if (!empty($response) && is_array($response)) {
		$markup = '<table border="0" id="network-sites">
			<tr>
				<th>Title</th>
				<th>Domain</th>
				<th class="last">BackLink</th>
			</tr>';
		foreach ($response as $site) {
			$markup .= '<tr id="network-site-' . $site['Id'] . '">
				<td>' . $site['Title'] . '</td>
				<td>' . $site['Domain'] . '</td>
				<td class="last">' . $site['BackLink'] . '</td>
			</tr>';
		}
		$markup .= '</table>';
		$form['existing_container']['Existing'] = array(
			'#markup' => $markup
		);
	} else {
		$form['existing_container']['NoneFound'] = array(
			'#markup' => '<h3>' . t('None found') . '</h3>'
		);
	}
	
	$form['container'] = array(
		'#type' => 'fieldset',
		'#title' => t('Add New'),
		'#description' => t('Add new network site.')
	);
	$form['container']['Title'] = array(
		'#type' => 'textfield',
		'#title' => t('Title')
	);
	$form['container']['Domain'] = array(
		'#type' => 'textfield',
		'#title' => t('Domain')
	);
	$form['container']['BackLink'] = array(
		'#type' => 'textfield',
		'#title' => t('BackLink')
	);
	
	
	$form['container']['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Update Network Settings')
	);
	$form['#submit'][] = 'medipass_network_settings_submit';
	
	return $form;
	
}

function medipass_network_settings_submit($form, &$form_state) {
	
	$data = $form_state['input'];
	
	$fields = array(
		'Title',
		'Domain',
		'BackLink'
	);
	
	foreach ($data as $key => $value) {
		if (!in_array($key, $fields)) {
			unset($data[$key]);
		}
	}
	
	$response = mediapass_api_request('network/list', array(
		'method' => 'POST',
		'data' => $data
	));
	
	if ($response) {
		drupal_set_message(t('Updated successfully!'));
	}
	
}