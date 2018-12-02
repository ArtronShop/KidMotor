Blockly.Blocks['kidmotor_set_speed'] = {
	init: function() {
		this.appendValueInput("speed")
			.setCheck("Number")
			.appendField(Blockly.Msg.KIDMOTOR["KidMotor    Set speed Chanel"])
			.appendField(new Blockly.FieldDropdown([["A","CH_A"], ["B","CH_B"]]), "ch")
			.appendField(Blockly.Msg.KIDMOTOR["to"]);
		this.appendDummyInput()
			.appendField("%");
		this.setInputsInline(true);
		this.setPreviousStatement(true, null);
		this.setNextStatement(true, null);
		this.setColour(45);
		this.setTooltip("Set speed of motor");
		this.setHelpUrl("htttps://www.ioxhop.com/");
	}
};

Blockly.Blocks['kidmotor_set_direction'] = {
	init: function() {
		this.appendDummyInput()
			.appendField(Blockly.Msg.KIDMOTOR["KidMotor    Set direction Chanel"])
			.appendField(new Blockly.FieldDropdown([["A","CH_A"], ["B","CH_B"]]), "ch")
			.appendField(Blockly.Msg.KIDMOTOR["to"])
			.appendField(new Blockly.FieldDropdown([[Blockly.Msg.KIDMOTOR["Stop"],"0"], [Blockly.Msg.KIDMOTOR["Forward"],"1"], [Blockly.Msg.KIDMOTOR["Reverse"],"2"], [Blockly.Msg.KIDMOTOR["Break"],"3"]]), "dir");
		this.setInputsInline(true);
		this.setPreviousStatement(true, null);
		this.setNextStatement(true, null);
		this.setColour(45);
		this.setTooltip("Set direction of motor");
		this.setHelpUrl("htttps://www.ioxhop.com/");
	}
};
