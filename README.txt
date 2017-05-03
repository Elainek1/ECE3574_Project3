Put Your design decisions here. What modules you defined and how they
interact to solve the ray tracing problem.

FOR GEOMETRY.HPP and GEOMETRY.CPP************

The geometry.hpp and geometry.cpp files were created to hold the classes of objects
These files contained the class called point which contains an x, y, and z values, as well as an int that defines which object it might be associated with

colorClass contains three ints of r, g, b to hold the values for each color,  In addition, there is a method called scaleBy which scales the values of r, g, b based on the input given

In addition there are classes for objects in the scene, this is for objects of type sphere and plane
The class is called shape and contains the type, center which is of type point, color which is of type colorClass, a lambert value, a radius value, and a normal which is of type point

There is a class called camera to hold the center, focus, normal, resolution, size values of the camera

There is a light class called light to hold the location and intensity of the lights in the scene

In addition, there are dot, normal, mag, diff, and normalize functions which are all operations that can be performed on the class type point

dot takes two points and dots the vector together to return a double

normal takes two points a center and an edge and computes the unit normal vector for that point on the sphere

mag takes in a point and computes the magnitude of the vector and returns a double

diff takes in two points start and end and computes the difference of the two (end - start) and returns this point

normalize takes in a point and normalizes the vector to be a unit vector and returns this result




FOR JSON.HPP and JSON.CPP****************

The json.hpp and json.cpp files were created to hold the class called file which will read in the json files and run the ray tracing alogorithm and save the resultant png

The file class has three vector members, one of type light to hold all the lights in the scene, one of type shape to hold all the objects in the scene, and one of type colorClass to hold all the pixel color values in the image

There are also an instance of camera and QImage pointer which is used to create the png output

The file class has a method called readJson which reads in the json files and parses the input by calling parseCamera, parseLights, and parse Objects which are all private methods of the file class

There is also a findPixelVal method that calls intersects and planeIntersect to compute the ray tracing algorithm to find the objects that are intersected at a given point.  Once the object is found the findPixelVal computes the pixelvalue and returns the colorClass type containing the appropriate value

renderImage() is a method in file class that calls the findPixelVal function which populates the pixColorArray containing all the pixel values.  This function then saves the image using QImage's save function that will write to the PNG

vtray.cpp creates an instance of file called json and reads the json file to parse.  If parsing succeeds then renderImage is called to generate the picture


************************************************************************

The four files were created to nicely divide up the appropriate code and make it more readable

I decided to use vectors to hold my objects and lights because I could use the push_back method and it would handle resizing the vector, which was nice since the objects and lights in a scene can vary.

In addition, a variable called rgbMax is used for autoexposure.  Basically if the value of r, g, or b is higher than the current set max value then it is updated.  This lets us know the max value and helps us scale the image and account for autoexposure correctly.


Lastly a function called setPngFilename is used to let the file class know exactly the image name that we want to save to.






