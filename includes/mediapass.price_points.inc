<?php

function mediapass_price_points($form, &$form_state) {
	
	$period_options = array(
		'1mo' => '1 Month',
		'3mo' => '3 Months',
		'6mo' => '6 Months',
		'1yr' => '1 Year'
	);
	
	$response = mediapass_api_request('price', array(
		'method' => 'GET',
		'params' => array(
			'Id' => variable_get('MP_user_ID')
		)
	));
	
	$subscription_model = (count($response) > 1) ? 'membership' : 'single';
	
	$form['container'] = array(
		'#type' => 'fieldset',
		'#title' => t('MediaPass Price Points'),
		'#description' => t('Manage your MediaPass price points.')
	);
	$form['container']['markup_intro'] = array(
		'#markup' => '<p>Set up Pricing to get started.</p>

		<p>First, choose your Subscription Model. Most websites will use Membership Access, to allow multiple pages or sections of content to be included in a subscription membership. The Single Article Access is for websites that wish to only charge for specific pieces of content. This option is similar to a pay-per-view model, and does not support ongoing memberships.</p>'
	);
	$form['container']['SubscriptionModel'] = array(
		'#type' => 'select',
		'#title' => t('My Subscription Model'),
		'#value' => $subscription_model,
		'#options' => array(
			'membership' => t('Membership'),
			'single' => t('Single')
		),
	);
	
	// MEMBERSHIP CONTAINER
	$form['membership_container'] = array(
		'#type' => 'fieldset',
		'#title' => t('Membership'),
		'#description' => 'Next, set up your Price Points. Your users will see three options when asked to sign-up for a subscription. Select the 3 subscription membership periods to choose from, and the corresponding unit price for each period. For example, if you want to charge $60 for a 6 month subscription, enter $10 for the unit Price.',
		'#attributes' => array(
			'style' => ($subscription_model != 'membership') ? 'display:none' : null
		),
	);
	$form['membership_container']['period_1'] = array(
		'#type' => 'select',
		'#options' => $period_options,
		'#title' => t('Period 1'),
		'#value' => _getPricePointValue($response, 0, 'period')
	);
	$form['membership_container']['price_1'] = array(
		'#type' => 'textfield',
		'#title' => t('Price 1'),
		'#suffix' => '<hr/>',
		'#value' => _getPricePointValue($response, 0, 'price')
	);
	$form['membership_container']['period_2'] = array(
		'#type' => 'select',
		'#options' => $period_options,
		'#title' => t('Period 2'),
		'#value' => _getPricePointValue($response, 1, 'period')
	);
	$form['membership_container']['price_2'] = array(
		'#type' => 'textfield',
		'#title' => t('Price 2'),
		'#suffix' => '<hr/>',
		'#value' => _getPricePointValue($response, 1, 'price')
	);
	$form['membership_container']['period_3'] = array(
		'#type' => 'select',
		'#options' => $period_options,
		'#title' => t('Period 3'),
		'#value' => _getPricePointValue($response, 2, 'period')
	);
	$form['membership_container']['price_3'] = array(
		'#type' => 'textfield',
		'#title' => t('Price 3'),
		'#value' => _getPricePointValue($response, 2, 'price')
	);
	
	
	// SINGLE CONTAINER
	$form['single_container'] = array(
		'#type' => 'fieldset',
		'#title' => t('Single Article Price'),
		'#attributes' => array(
			'style' => ($subscription_model != 'single') ? 'display:none' : null
		),
	);
	$form['single_container']['price'] = array(
		'#type' => 'textfield',
		'#title' => t('Single Article Price ($)'),
		'#value' => ($subscription_model == 'single') ? _getPricePointValue($response, 0, 'price') : null
	);
	$form['single_container']['default'] = array(
		'#type' => 'checkbox',
		'#title' => t('Set this price point set as my default active price set.'),
		'#suffix' => '<p><strong>(Note: This will stop and overide any current price point test.)</strong></p>'
	);
	
	$form['closing_markup'] = array(
		'#markup' => '<p>You can also create different Price Point Sets for testing purposes. Go to Price Point Tests in the Customization section to learn more.</p>
		<script type="text/javascript" charset="utf-8">
			jQuery(document).ready(function($){
				$("#edit-subscriptionmodel").bind("change", function(e) {
					switch ($(e.target).val()) {
						case "membership":
							$("#edit-membership-container").show();
							$("#edit-single-container").hide();
							break;

						case "single":
							$("#edit-membership-container").hide();
							$("#edit-single-container").show();
							break;
					}
				});
			});
		</script>'
	);
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Update Price Points')
	);
	
	$form['#submit'][] = 'medipass_price_points_submit';
	
	return $form;
}

function medipass_price_points_submit($form, &$form_state) {
	
	$data = $form_state['input'];
	
	$increment_map = array(
		'1mo' => array(
			'Length' => 1,
			'Increment' => 2592000
		),
		'3mo' => array(
			'Length' => 3,
			'Increment' => 2592000
		),
		'6mo' => array(
			'Length' => 6,
			'Increment' => 2592000
		),
		'1yr' => array(
			'Length' => 1,
			'Increment' => 31104000
		)
	);
	
	$price_model = array();
	
	switch ($data['SubscriptionModel']) {
		case 'membership':
			foreach (array(1,2,3) as $index) {
				$price_model[$index] = $increment_map[$data['period_' . $index]];
				$price_model[$index]['Price'] = $data['price_'.$index];
				$price_model[$index]['Type'] = 0;
			}
			break;
		
		case 'single':
			$price_model[] = array(
				'Type' => 1,
				'Length' => 1,
				'Increment' => 31104000,
				'Price' => $data['price']
			);
			break;
	}
	
	$data = array(
		'Id' => (int) variable_get('MP_user_ID'),
		'Active' => 1,
		'PriceModel' => $price_model
	);
	
	$response = mediapass_api_request('price', array(
		'method' => 'POST',
		'data' => $data
	));
	
	if ($response) {
		drupal_set_message(t('Updated successfully!'));
	}
}

function _getPricePointValue($values=array(), $index=0, $key='period') {
	
	// Increment: 2592000 for month, 31104000 for year, 86400 for day.
	// Type: 0 for memebership, 1 for single article
	$increment_map = array(
		2592000 => '1mo',
		7776000 => '3mo',
		15552000 => '6mo',
		31104000 => '1yr'
	);
	
	if (!empty($values[$index])) {
		
		$value = $values[$index];
		
		if ($key == 'price') {
			return $value['Price'];
		} elseif ($key == 'period') {
			return $increment_map[($value['Length'] * $value['Increment'])];
		}
		
	}
	
	return null;
	
}