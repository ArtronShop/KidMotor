var ADDONS_KIDMOTOR_CLASS_BEGIN = 'DEV_I2C1.KidMotor(0, 0x65)';

Blockly.JavaScript['kidmotor_set_speed'] = function(block) {
	var dropdown_ch = block.getFieldValue('ch');
	var value_speed = Blockly.JavaScript.valueToCode(block, 'speed', Blockly.JavaScript.ORDER_ATOMIC);
	var code = ADDONS_KIDMOTOR_CLASS_BEGIN + '.setSpeed(' + dropdown_ch + ', ' + value_speed + ' * 255.0 / 100.0);\n';
	return code;
};

Blockly.JavaScript['kidmotor_set_direction'] = function(block) {
	var dropdown_ch = block.getFieldValue('ch');
	var dropdown_dir = block.getFieldValue('dir');
	var code = ADDONS_KIDMOTOR_CLASS_BEGIN + '.setMotor(' + dropdown_ch + ', ' + dropdown_dir + ');\n';
	return code;
};
