<p align=center>
<img src="/Images/IMG_5634.jpg" alt="" width="50%" height="50%" />
</p>

# Overview

A new evolution of the famous Olav Kallhovd CG Scale. For this release, I did a merge of :
* the original project from Olav here: https://github.com/olkal/CG_scale
* the fork from Alain Désandré here: https://github.com/adesandr/CG_Scale_OlkalBreakout
* the fork from Stephan Gamp here: https://github.com/sgamp/CG_Scale_modifiedSG


I didn't change everything, of course, the main part of the original programm regarding the scale management is kept, because it works very well and provides a very accurate result. I have included a new library, very efficient to capture the key values, from Vedat Ozan Oner (https://www.codeproject.com/Tips/709109/ADKeyboard-Library-for-Arduino).

# Dimensions
<p align=center>
<img src="/Images/CG Scale F3F.jpg" alt="" width="50%" height="50%" />
</p>
<p align=center>
<img src="/Images/CG Scale F5J.jpg" alt="" width="50%" height="50%" />
</p>

# What'n new with this release 

## For the scale part :

* a separate box has been designed, in which a keyboard and the OLED display take place.
* the main frame has been optimized to host correctly the 9v battery.
* a wire clamp system has been added, to save the wires with an external control box
* a specific PCB has been designed to reduce the size of the electronic components and have less wires into the scale
* the new design enables the change of the load cell (just need to unweld the old, and weld the new one.

## For the software part :

* a interactive menu has been added for the initial configuration
* the autocalibration (a very good idea from Stephen indeed !) has been optimized to win time and accuracy
* it is possible to choose the type of support: little, medium, large ; for the moment, they all have the same characteristics, but in a later release, I will add bigger support for bigger model
* the tare configuration menu is simpler and consumes less memory
* two lang are available: [English](/User_Manual.md) and [French](/Manuel_utilisateur.md)
* the keyboard management is done by the excellent library from Vedat, and eliminate the issues when setting the values of key pressed.

## Assembly manual

The assembly instructions are perfectly described into the [Alain's project](https://github.com/adesandr/CG_Scale_OlkalBreakout), so I suggest to follow his guide.
You can also find useful instructions into the manual from [Stephen's projet](https://github.com/sgamp/CG_Scale_modifiedSG).

# Evolution to do after

* use a bigger display, will be more confortable, such as a 1.3" OLED (very easy to do)
* do a rework of the frame to accept bigger supports, for bigger models 
* create new supports for bigger models like this : 
<p align=center>
<img src="/Images/CG Scale XL.jpg" alt="" width="50%" height="50%" />
</p>

* any other improvement that everyone wants to add !



