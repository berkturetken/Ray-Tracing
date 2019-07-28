Computer Graphics, Assignment 4 - Project
=============================================

This project is done for Computer Graphics course in Uppsala University, Sweden as a last assignment and course project by Berk Türetken, Buket Karakaş, and Buse Çarık.

The aim of the project is to implement CPU-based ray tracing (or path tracing) for global illumination.

We've followed the structure of the "Ray Tracing In a Weekend" book, and started from Chapter 6.

We, as a group, completed the following tasks:

• Implement anti-aliasing by adding per-sample jilter to the camera rays.

• Implement rendering of diffuse surfaces by tracing secondary ray for each camera ray hitting a surface. The light source should be a hemisphere light (the sky) and your ray tracing should support at least three random bounces.

• Add a few more spheres and more surface types to your scene. You should implement at least materials for metallic (reflective) and glass (refractive) surfaces. If you have time, feel free to add other materials as well, such as glossy or emissive surfaces.

• Add an ImGUI window for controlling rendering parameters such as material properties, anti-aliasing, number of bounces, etc. It should also be possible to display the surface normals and toggle gamma correction on/off. Note that gamma correction in the code is performed by the fragment shader used to display the ray traced image.

• When all the above is implemented and work correctly: add one of the provided low-polygon meshes (or some other mesh of similary complexity) to your scene. You will notice that the rendering becomes very slow... To improve this, compute and store the bounding box of the mesh and add a bounding volume test (sphere or box) to the tracing step. Notice the speedup!

