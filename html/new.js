		var nameBusy = ".Busy0";
		cp = ColorPicker(
		document.getElementById('pcr'),
		document.getElementById('picker'), 
		function(hex, hsv, rgb, mousePicker, mousepcr) {
			currentColor = hex;
			ColorPicker.positionIndicators( document.getElementById('pcr-indicator'), document.getElementById('picker-indicator'), mousepcr, mousePicker);
            document.getElementById('hex').innerHTML = hex;
			document.getElementById('rgb').innerHTML = 'rgb(' + rgb.r.toFixed() + ',' + rgb.g.toFixed() + ',' + rgb.b.toFixed() + ')';
			document.getElementById('hsv').innerHTML = 'hsv(' + hsv.h.toFixed() + ',' + hsv.s.toFixed(2) + ',' + hsv.v.toFixed(2) + ')';
                     
			document.getElementById('pcr_bg').style.backgroundColor = hex;
		 var matches = document.querySelectorAll(nameBusy);
			for(var i = 0; i < matches.length; i++) {
        matches[i].style.backgroundColor = hex;
      }
		}
		);
		$('input[name="radio"]').click(function() 
		{
			$('.active').removeClass('active');
			$(this).addClass('active');
			let id = $(this).attr('id');
			nameBusy = ".Busy" + id;
		})
		
		cp.setHex('#D4EDFB');
		var $active = $('.active').attr("checked","checked");