A new evolution of the famous Olkal CG Scale. For this release, I did a merge of :
* the original project from Olav Kallhovd here: https://github.com/olkal/CG_scale
* the fork from Alain Desembré here: https://github.com/adesandr/CG_Scale_OlkalBreakout
* the fork from Stephan Gamp here: https://github.com/sgamp/CG_Scale_modifiedSG


I didn't change everything, of course, the main part of the original programm regarding the scale management is kept, because it works very well and provides a very accurate result. I have included a new library, very efficient to capture the key values, from Vedat Ozan Oner (https://www.codeproject.com/Tips/709109/ADKeyboard-Library-for-Arduino).

What'n new with this release 

For the scale part :

* a separate box has been designed, in which a keyboard and the OLED display take place.
* the main frame has been optimized to host correctly the 9v battery.
* a wire clamp system has been added, to save the wires
* a specific PCB has been designed to reduce the size of the components and have less wires into the box
* the new design enables the change of the load cell

For the software part :

* a interactive menu has been added for the initial configuration
* the autocalibration is optimized to take a very short time
* it is possible to choose the type of support (little, medium, large)
* the tare configuration menu is simpler and takes less memory
* two lang are available: English and French


The assembly instructions are perfectly described into the Alain Désambré project, so I suggest to follow his guide.
