<?php
function mediapass_benefits($form, &$form_state) {
	
	_checkSetup();
	
	if (empty($form_state['input'])) {
		$response['Benefit'] = mediapass_api_request('benefit', array(
			'method' => 'GET',
			'params' => array(
				'Id' => variable_get('MP_user_ID')
			)
		));
	} else {
		$response = $form_state['input'];
	}
	
	$logo = mediapass_api_request('logo', array(
		'method' => 'GET',
		'params' => array(
			'Id' => variable_get('MP_user_ID')
		)
	));
	
	$values = array_merge(array(
		'Benefit' => null
	), (array) $response);
	
	$form['container'] = array(
		'#type' => 'fieldset',
		'#title' => t('MediaPass Benefits'),
		'#description' => t('Manage your MediaPass benefits information.')
	);
	
	$logo_prefix = '<p>Upload a logo to customize the look and feel of your Page Overlay and Video Overlay subscription options. Your logo must be a jpg. See examples below.';
	
	if (!empty($logo)) {
		$logo_prefix .= '<img src="' . $logo . '"/></p>';
	} else {
		$logo_prefix .= '</p>';
	}
	
	$form['container']['Logo'] = array(
		'#type' => 'file',
		'#title' => t('Benefits Logo'),
		'#value' => null,
		'#prefix' => $logo_prefix,
		'#suffix' => '<table border="0">
			<tr>
				<td valign="top" width="175"><strong>Your logo <br/>on Page Overlay option</strong></td>
				<td><img src="' . url() . drupal_get_path('module', 'mediapass') . '/images/update-logo-inPage.jpg" width="436" height="246" alt="Update Logo InPage"></td>
			</tr>
		</table>'
	);
	$form['container']['Benefit'] = array(
		'#type' => 'textarea',
		'#title' => t('Benefits'),
		'#value' => $values['Benefit'],
		'#prefix' => '<p>Customize your messaging by marketing the benefits of becoming a premium subscriber. Enter the text to be displayed to your users in the Member Benefits section of the subscription option. See examples below.</p>

		<p>TIP: Benefits are displayed as bullet points, so separate each benefit on a new line in the text box.</p>',
		'#suffix' => '<table border="0">
			<tr>
				<td valign="middle" width="175"><strong>Benefits Text <br/>on Page Overlay option</strong></td>
				<td><img src="' . url() . drupal_get_path('module', 'mediapass') . '/images/benefits-pageOverlay.jpg" alt="Overlay Benefits Text" style="padding-left:15px;"></td>
			</tr>
			<tr>
				<td valign="middle" width="175"><strong>Benefits Text <br/>on In-Page option</strong></td>
				<td><img src="' . url() . drupal_get_path('module', 'mediapass') . '/images/benefits-inpage.jpg" alt="In Page Benefits Text"></td>
			</tr>
		</table>'
	);
	$form['container']['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Update Benefits')
	);
	$form['#submit'][] = 'medipass_benefits_submit';
	
	return $form;
	
}

function medipass_benefits_submit($form, &$form_state) {
	
	$file = file_save_upload('Logo', array('mediapass_file_validate'), 'public://', FILE_EXISTS_RENAME);
	
	if (!empty($file)) {
		$doc_url = file_create_url($file->uri);
		$file->status = FILE_STATUS_PERMANENT;
		file_save($file);
		
		$response = mediapass_api_request('benefit', array(
			'method' => 'POST',
			'data' => array(
				'Id' => variable_get('MP_user_ID'),
				'Url' => $doc_url
			)
		));
		
	}
	
	$data = $form_state['values'];
	
	$fields = array(
		'Benefit' => null
	);
	
	foreach ($data as $key => $value) {
		if (!in_array($key, $fields)) {
			unset($data[$key]);
		}
	}
	
	$response = mediapass_api_request('benefit', array(
		'method' => 'POST',
		'data' => $data
	));
	
	if ($response) {
		drupal_set_message(t('Updated successfully!'));
	}
	
}

function mediapass_file_validate($file) {
	
	$errors = array();
	
	$pathinfo = pathinfo($file->filename);
	
	if (!in_array($pathinfo['extension'], array('jpg', 'jpeg'))) {
		$errors[] = t("This file is not a .jpg file.");
	}
	
	return $errors;
	
}