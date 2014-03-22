Brain

Sequence sequence //current sequence being processed

Event wait_for_input(timePeriod){
	while {
		if (input is detected) {
			input_detected(input)) //probably set a new sequence and push/process it
			return process_sequence()
		}
		else if(sequence is empty){
			if(timePeriod has passed){
				random_sequence() //times-up start a new sequence
				return process_sequence()
			}
		}
		else return process_sequence()
	}
}

Event process_sequence(){
	for each event in sequence{
		pop_event_off_sequence()
		return the event
	}
}