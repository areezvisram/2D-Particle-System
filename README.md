## **2D Particle System**

### **Program Execution**
Using the makefile, the program can be executed via:
* `main`: Run `make main && ./main`

To remove the executables run:
* `clean`: Run `make clean`

### **Main Program**
**PLEASE NOTE: The bindings are CASE SENSITIVE**

A list of the key bindings and the action they pertain to:
**MOUSE COMMANDS**
* Left Mouse Button + Hold - Attract particles within range
* Right Mouse Button + Hold - Repel particles within range

**KEYBOARD COMMANDS (case sensitive)**
* a - Add new particle at mouse position
* d - Delete particle nearest to mouse position
* "+" - Increase global range that particles can attract/repel within
* "-" - Increase global range that particles can attract/repel within
* UP ARROW - Increase global speed that particles move with
* DOWN ARROW - Decrease global speed that particles move with
* Space bar - Pause the simulation
* r - Reset animation with brand new particles
* q - Quit Program
* ESC - Quit Program
* c - Toggle display of range of each individual particle
* C - Toggle display of global range of mouse cursor
* b - Arrange the particles in a circle shape
* z - Arrange the particles in a flower shape
* v - Toggle display of direction vectors of each particle
#### **Features:**
**Improved Physics**
* Real physics is implemented in this program
* This means that the particles move with real acceleration, which is determined by the forces acting on them as well as their mass and speed
* Attracting and repelling the particles will apply a force to them. For attracting,
if the force is strong enough, the particle will begin to orbit around the cursor. If it's not strong enough, the particle will be able to escape the orbit after attracting
* There is also a friction force being applied to each particle at all times it is moving, in the opposite direction of it's velocity
* This means that eventually every particle will come to a stop after it has begun moving

**Direction Vectors**
* To get a better sense of the forces that are being applied to each particle, direction vectors can be toggled on and off using the 'v' key
* The vectors show the direction of the acceleration vector for each particle that is in motion, which makes it easier to see the improved physics explained above

**Range Radii**
* Clicking the 'c' key toggles on and off the display of range radii for each particle
* Turning this on shows the pre-defined range of each particle
* This makes it easier to see when a particle is in range of the cursor
* Clicking the 'C' key toggles on and off the display of the range radius for the cursor
* Initially it will not show a radius, that is because the initial global range is 0
* The global range can be increased with the '+' key, upon which a range radius for the cursor will appear
* With both of these on, it is easy to see when a particle is in range. A particle is in range anytime the cursor radius intersects the particle radius

**Predefined Animations**
* There are 2 predefined animations which arrange the particles in specific shapes
* Pressing the 'b' key at any time will start to move the particles into directions where they eventually stop to form a circle of set radius
* The particles can still be attracted and repelled at any time during the animation, or after the animation has finished
* Pressing the 'z' key at any time will start to move the particles into directions where they eventually stop to form a flower shape

**Wall Collisions**
* Upon attraction or repelling, particles do not just fly off the screen
* There is collision detection with each wall, meaning the particles bounce off the walls and never leave the screen