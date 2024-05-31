# brick-breaker
Brick Breaker made in C++ (OpenGL) as a semester project for OOP module at FAST-NU.

## How to compile:
1. Clone the repository in Linux Ubuntu Distro
2. Run the following commands:
   (i)   sudo apt update\n
   (ii)  sudo apt-install build-essential\n
   (iii) sudo apt-get install freeglut3-dev glew-utils libglew-dev libfreeimage-dev
3. A Game exe will be created that can be executed by **./game**

## Controls:
Use the **mouse** to control the primary paddle\n
Arrow Keys can be used to control the secondary paddle (only in Level 3)\n
Each Brick has different lives and PowerUps based on randomly assigned initial attributes.

## Understanding the code:
1. A primary class **Paddle** that inherits from the **Grid** class and has other class objects made in composition.
2. **Grid** class only contains constant so inheriting it multiple times in other classes is only purposed for accessing constants and drawing a layout of the grid.
3. By creating a **Brick** class pointer in Paddle, Levels are created and drawn using polymorphism.
4. Brick class contains mechanism for collision with the Bricks and activating PowerUps when each of the bricks gets destroyed.
5. The **Ball** class contains all the functionality of the Ball including: setting the deflection angles and movement. The collision with Paddles is handled in Paddle class itself.
6. A **Point** class is used by every class to set the coordinates of their objects easily.
