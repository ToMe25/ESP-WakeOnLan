var device_select
var device_input
var target_select
var target_input

function init() {
	device_select = document.getElementById("device")
	device_select.onchange = onchange
	device_input = document.getElementById("custom_device")
	target_select = document.getElementById("target")
	target_select.onchange = onchange
	target_input = document.getElementById("custom_target")

	onchange()
}

function onchange() {
	if (device_select.selectedOptions[0].value == "Other") {
		device_input.hidden = false
	} else {
		device_input.hidden = true
	}

	if (target_select.selectedOptions[0].value == "Other") {
		target_input.hidden = false
	} else {
		target_input.hidden = true
	}
}

window.onload = init
