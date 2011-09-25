<?php
function mediapass_summary() {
	
	$path_pieces = explode('/', current_path());
	$last = end($path_pieces);
	
	$selected = null;
	if (in_array($last, array('today', 'this_month', 'this_year'))) {
		$selected = $last;
	}
	
	if (!empty($selected)) {
		$stats = mediapass_api_request('report/summary/stats', array(
			'method' => 'GET',
			'params' => array(
				variable_get('MP_user_ID'),
				$selected
			)
		));
	} else {
		$stats = mediapass_api_request('report/summary/stats', array(
			'method' => 'GET',
			'params' => array(
				variable_get('MP_user_ID')
			)
		));
	}

	$earning = mediapass_api_request('report/summary/earning', array(
		'method' => 'GET',
		'params' => array(
			variable_get('MP_user_ID')
		)
	));
	
	$data = array(
		'stats' => $stats,
		'earning' => $earning
	);
	
	$select = '<select name="period" id="period">';
	foreach (array('today' => t('Today'), 'this_month' => t('This Month'), 'this_year' => t('This Year')) as $value => $label) {
		$select .= '<option value="' . $value . '" ' . (($selected == $value) ? ' selected="selected"' : null) . '>' . $label . '</option>';
	}
	$select .= '</select>';
	
$return = <<<HEREDOC
	<h2 class="header">Summary Report</h2>
	
	<table border="0" id="earning-summary">
		<tr>
			<th colspan="2">Earning Summary</th>
		</tr>
		<tr>
			<td width="65%">Total Earned:</td>
			<td width="35%">\${$data['earning']['Commissions']}</td>
		</tr>
		<tr>
			<td width="65%">Total Paid:</td>
			<td width="35%">\${$data['earning']['Paid']}</td>
		</tr>		
		<tr>
			<td width="65%">Current Balance:</td>
			<td width="35%">\${$data['earning']['Balance']}</td>
		</tr>
	</table>
	
	<table border="0" id="stats">
		<tr>
			<th colspan="2">
				Stats for {$select}
			</th>
		</tr>
		<tr>
			<td width="65%">Sales:</td>
			<td width="35%">{$data['stats']['Sales']}</td>
		</tr>
		<tr>
			<td width="65%">Commissions:</td>
			<td width="35%">\${$data['stats']['Commissions']}</td>
		</tr>		
		<tr>
			<td width="65%">Impressions:</td>
			<td width="35%">{$data['stats']['Impressions']}</td>
		</tr>
	</table>
	<script type="text/javascript" charset="utf-8">
		
		var periods = new Array("today","this_month","this_year");
		
		function in_array(string, array) {
	   		for (i = 0; i < array.length; i++) {
	      		if(array[i] == string) {
	         		return true;
	      		}
	   		}
			return false;
		}
		
		jQuery(document).ready(function(){
			
			// Redirect based on time period on Summary Report view
			jQuery('#period').bind('change', function(e){
				
				var url_array = window.location.href.split('/');
				var existing = url_array[url_array.length - 1];
				
				if(in_array(existing, periods)) {
					
					window.location = window.location.href.replace(existing, jQuery(this).val());
					
				} else {
					
					if (window.location.href.indexOf('?') != -1) {
 						new_location = window.location.href.split('?');
						window.location = new_location[0] + '/' + jQuery(this).val();
					} else {
						window.location = window.location.href + '/' + jQuery(this).val();
					}
					
				}

			});
		});
	</script>
HEREDOC;
return $return;
}