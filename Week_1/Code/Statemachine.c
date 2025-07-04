typedef enum {
    STATE_IDLE,
    STATE_RECEIVING,
    STATE_SENDING
} chat_state_t;

while (1) {
    switch (state) {
        case STATE_IDLE:
            if (has_incoming_msg())
                state = STATE_RECEIVING;
            else if (has_outgoing_msg())
                state = STATE_SENDING;
            break;
        case STATE_RECEIVING:
            process_incoming();
            state = STATE_IDLE;
            break;
        case STATE_SENDING:
            send_msg();
            state = STATE_IDLE;
            break;
    }
}

