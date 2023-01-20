# webcrane

A webcrane is a self-invented special type of cable-guided robot. It has spread out pulleys which form a cable arrangement that resembles a spider web. The idea behind the webcrane is a great increase in load capacity compared to a normal cable-guided robot.

To move the crane, the lengths of the attached cable must be adjusted in precisely. I wrote a C code which does the therefore needed calculations and would potentially be able to control a webcrane. However, the webcrane is a mechanically complex system so instead of actually building a real demo model I decided to make a Simscape simulation to test the C control code as a Software-in-the-loop simulation and see if it is actually able to control a webcrane.