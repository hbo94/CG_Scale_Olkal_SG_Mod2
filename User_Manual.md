# User manual

## Introduction
An Arduino program is structured in 3 parts:
* declarations, include, etc.
* the "setup" program, executed only once
* the main program, executed in a loop.

The menu for configuring the balance is located in the "setup" step and is therefore only accessible when the balance is powered on. Once you have left the menu, you go to the main program.


## The keyboard
! [keyboard] (/Images/CGScale-keyboard-en.jpg)

## Initial setting

### Main Menu
When the scale starts up, the following menu is displayed: </br>
**`Configuration`** </br>
`Support option` </br>
`Calibration` </br>
`> Quit` </br>

In this first menu level, the `Enter` key is used to select an action. `Quit` will launch the balance's main program.
In the sub-menu, the selection of an option is done with the `Right` key, the` Left` key allowing it to return to the main menu.

### Support choice menu
**`Support: small`** </br>
`Small` </br>
`Medium` </br>
`> Large` </br>

This submenu allows you to choose the type of support. For now, the three available supports all have the same characteristics, so there is no need to change the support type. This will be useful in a future version of the software, with larger supports.

### Calibration menu
**`Calibration`** </br>
`> Config. tare` </br>
`Front calibration` </br>
`Back calibration </br>

This submenu allows 3 things:
* define the reference tare weight that you will use to calibrate the scales
* calibrate the front scale 
* calibrate the back scale </br>

Each option is selected with the `Right` key.

#### Define the tare weight
The screen then displays the last weight defined (it is stored in the permanent memory of the scale). To modify it, the following keys are used:
* `Left` and` Right` to change the value of the increment
* `+` and `-` to increase or decrease the weight of the increment value
* `Enter` for 5s to save the weight in the balance memory, and return to the previous submenu.

#### Calibrate the scale before
The operation is very simple:
* place the tare weight on the front support
* press the `Enter` key for 5s to start the calibration </br>

The operation takes a few seconds or even a minute, but not longer. If it takes too long, there is a problem, and it is best to restart the scale.

#### Calibrate the rear scale
Same operation as for the front scale. The only difference, succeeding in balancing the tare on the blocks, the exercise can require a certain skill and cause some temporary annoyances.
Return to the main menu with the `Left` key.

## Using the scale

Once calibrated, the scale operates on its own, place a model on the supports, and the weight and CG readings are done in seconds.

** Recommendations for proper operation **
* place the scale on a stable and level surface
* do not touch the balance supports during its start-up phase
* Avoid weighing a model when it is windy, the reading will not be stable because the model is moving.

## Problems encountered

### Stability of values ​​returned by a balance
* the strain gauges are delivered with long enough wires, which must be shortened as much as possible to reduce the "noise", this "noise" will prevent the HX711 amplifiers from "reading" stable values ​​and the calibration will not work.
* to attenuate even more the "noise", it is necessary to twist the wires of the strain gauges.

### When using
* It can sometimes happen that at startup, the program "freezes". I don't know why, at compile time, everything is perfect, no alerts, the available memory is more than sufficient. I put it down to electronic components. But that's not a big deal, just restart the scale, and it starts again.
