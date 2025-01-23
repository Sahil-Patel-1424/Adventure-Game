// libraries
#include <Toggle.h>

// initialize variables

// LEDs
int led_pin_number_1[] = {7, 8, 9, 10, 11, 12, 13};
int number_of_leds_1 = sizeof(led_pin_number_1) / sizeof(led_pin_number_1[0]);
int led_pin_number_2[] = {5, 6};
int number_of_leds_2 = sizeof(led_pin_number_2) / sizeof(led_pin_number_2[0]);

// buttons
int button_pin_number[] = {24, 25, 26, 27};
int number_of_buttons = sizeof(button_pin_number) / sizeof(button_pin_number[0]);
Toggle north_button;
Toggle west_button;
Toggle south_button;
Toggle east_button;

// time
int time = 500;

// list of states in the first system
enum {CAVE_OF_CACOPHONY, TWISTY_TUNNEL, RAPID_RIVER, SECRET_SWORD_STASH, DRAGONS_DEN, GRIEVOUS_GRAVEYARD, VICTORY_VAULT} current_state;

// list of states in the second system
enum {NO_SWORD, HAS_SWORD} current_state_1;

// this function runs the finite state machine
void run_finite_state_machine() {
  // update the buttons' states
  north_button.poll();
  west_button.poll();
  south_button.poll();
  east_button.poll();

  // this switch statement will help us determine which state
  // we are in in order to determine what actions we must do
  switch (current_state) {
    // if we are in the CAVE OF CACOPHONY state, then we press the
    // east button in order to move to the TWISTY TUNNEL state
    case CAVE_OF_CACOPHONY:
      if (east_button.onPress()) {
        // turn off the CAVE OF CACOPHONY LED
        turn_off_LEDs();

        // turn on the TWISTY TUNNEL LED
        digitalWrite(led_pin_number_1[1], HIGH);

        // set the current state to the next state TWISTY TUNNEL
        current_state = TWISTY_TUNNEL;
      }
      break;

    // if we are in the TWIST TUNNEL state, then we press the west button
    // in order to move to the CAVE OF CACOPHONY state or the south button
    // in order to move to the RAPID RIVER state
    case TWISTY_TUNNEL:
      if (west_button.onPress()) {
        // turn off the TWISTY TUNNEL LED
        turn_off_LEDs();

        // turn on the CAVE OF CACOPHONY LED
        digitalWrite(led_pin_number_1[0], HIGH);

        // set the current state to the next state CAVE OF CACOPHONY
        current_state = CAVE_OF_CACOPHONY;
      }
      else if (south_button.onPress()) {
        // turn off the TWISTY TUNNEL LED
        turn_off_LEDs();

        // turn on the RAPID RIVER LED
        digitalWrite(led_pin_number_1[3], HIGH);

        // set the current state to the next state RAPID RIVER
        current_state = RAPID_RIVER;
      }
      break;

    // if we are in the RAPID RIVER state, then we press the north button
    // in order to move to the TWISTY TUNNEL state, the west button in order
    // to move to the SECRET SWORD STASH state, or the east button in order
    // to move to the DRAGONS DEN state
    case RAPID_RIVER:
      if (north_button.onPress()) {
        // turn off the RAPID RIVER LED
        turn_off_LEDs();

        // turn on the TWISTY TUNNEL LED
        digitalWrite(led_pin_number_1[1], HIGH);

        // set the current state to the next state TWISTY TUNNEL
        current_state = TWISTY_TUNNEL;
      }
      else if (west_button.onPress()) {
        // set the current state to the next state HAS SWORD
        current_state_1 = HAS_SWORD;

        // turn off the RAPID RIVER LED and NO SWORD LED
        turn_off_LEDs();

        // turn on the SECRET SWORD STASH LED
        digitalWrite(led_pin_number_1[2], HIGH);

        // turn on the HAS SWORD LED
        digitalWrite(led_pin_number_2[0], HIGH);

        // set the current state to the next state SECRET SWORD STASH
        current_state = SECRET_SWORD_STASH;
      }
      else if (east_button.onPress()) {
        // turn off the RAPID RIVER LED
        turn_off_LEDs();

        // turn on the DRAGONS DEN LED
        digitalWrite(led_pin_number_1[4], HIGH);

        // set the current state to the next state DRAGONS DEN
        current_state = DRAGONS_DEN;
      }
      break;

    // if we are in the SECRET SWORD STASH state, then we press
    // the west button in order to move to the RAPID RIVER state
    case SECRET_SWORD_STASH:
      if (east_button.onPress()) {
        // turn off the SECRET SWORD STASH LED
        turn_off_LEDs();

        // turn on the RAPID RIVER LED
        digitalWrite(led_pin_number_1[3], HIGH);

        // set the current state to the next state RAPID RIVER
        current_state = RAPID_RIVER;
      }
      break;

    // if we are in the DRAGONS DEN state, we would go to the VICTORY VAULT state if we have the secret sword or we would go to the GRIEVOUS GRAVEYARD state if we don't have the secret sword
    case DRAGONS_DEN:
      // this is to make it look like the game is determining if the user won or lost
      for (int i = 0; i < 3; i++){
        digitalWrite(led_pin_number_1[4], HIGH);
        delay(time);
        digitalWrite(led_pin_number_1[4], LOW);
        delay(time);
      }

      // this is to keep the user in suspense
      digitalWrite(led_pin_number_1[4], HIGH);
      delay(1000);

      // if the user has the sword, then they win the game
      // if the user does not have the sword, then they lose the game
      switch(current_state_1) {
        case NO_SWORD:
          // set the current state to GRIEVOUS GRAVEYARD
          current_state = GRIEVOUS_GRAVEYARD;
          break;

        case HAS_SWORD:
          // set the current state to VICTORY VAULT
          current_state = VICTORY_VAULT;
          break;
      }
      break;

    // if the user is in this state, then they lost the game
    case GRIEVOUS_GRAVEYARD:
      // turn off the DRAGONS DEN LED
      digitalWrite(led_pin_number_1[4], LOW);

      // turn on the GRIEVOUS GRAVEYARD LED
      digitalWrite(led_pin_number_1[5], HIGH);
      delay(time);

      // this is to signify that the user lost the game
      for (int i = 0; i < 4; i++) {
        digitalWrite(led_pin_number_1[5], HIGH);
        delay(time);
        digitalWrite(led_pin_number_1[5], LOW);
        delay(time);
      }

      // set current states to CAVE OF CACOPHONY and NO SWORD
      current_state = CAVE_OF_CACOPHONY;
      current_state_1 = NO_SWORD;

      // reset the game
      turn_off_LEDs();
      delay(time);
      digitalWrite(led_pin_number_1[0], HIGH);
      digitalWrite(led_pin_number_2[1], HIGH);
      break;

    // if the user is in this state, then they won the game
    case VICTORY_VAULT:
      // turn off the DRAGONS DEN LED
      digitalWrite(led_pin_number_1[4], LOW);

      // turn on the VICTORY VAULT LED
      digitalWrite(led_pin_number_1[6], HIGH);
      delay(time);

      // this is to signify that the user won the game
      for (int i = 0; i < 4; i++) {
        digitalWrite(led_pin_number_1[6], HIGH);
        delay(time);
        digitalWrite(led_pin_number_1[6], LOW);
        delay(time);
      }

      // set current states to CAVE OF CACOPHONY and NO SWORD
      current_state = CAVE_OF_CACOPHONY;
      current_state_1 = NO_SWORD;

      // reset the game
      turn_off_LEDs();
      delay(time);
      digitalWrite(led_pin_number_1[0], HIGH);
      digitalWrite(led_pin_number_2[1], HIGH);
      break;
  }
}

// this function will turn off all LEDs
void turn_off_LEDs() {
  // since we are turning off an LED, it will be easier to code
  // this function in a way that turns off all LEDs at once
  for (int i = 0; i < number_of_leds_1; i++) {
    digitalWrite(led_pin_number_1[i], LOW);
  }

  // if user has the sword then turn off NO SWORD LED
  // else turn off the HAS SWORD LED
  switch (current_state_1) {
    case NO_SWORD:
      digitalWrite(led_pin_number_2[0], LOW);
      break;

    case HAS_SWORD:
      digitalWrite(led_pin_number_2[1], LOW);
      break;
  }
}

void setup() {
  // put your setup code here, to run once:

  // create serial monitor
  Serial.begin(9600);

  // set LED pins as output signals
  for (int i = 0; i < number_of_leds_1; i++) {
    pinMode(led_pin_number_1[i], OUTPUT);
  }

  for (int i = 0; i < number_of_leds_2; i++) {
    pinMode(led_pin_number_2[i], OUTPUT);
  }

  // set button pins as input signals
  north_button.begin(button_pin_number[3]);
  west_button.begin(button_pin_number[2]);
  south_button.begin(button_pin_number[1]);
  east_button.begin(button_pin_number[0]);

  // initially have all LEDs off and have CAVE OF CACOPHONY LED on
  turn_off_LEDs();
  digitalWrite(led_pin_number_1[0], HIGH);

  // have NO SWORD LED on
  digitalWrite(led_pin_number_2[1], HIGH);

  // set the initial states in finite state machine as CAVE OF CACOPHONY and NO SWORD
  current_state = CAVE_OF_CACOPHONY;
  current_state_1 = NO_SWORD;
}

void loop() {
  // put your main code here, to run repeatedly:

  // run the finite state machine where it
  // gets updated based on possible events
  run_finite_state_machine();
}
