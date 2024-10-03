# Voronoi diagram 
My implementation of the [Voronoi diagram](https://en.wikipedia.org/wiki/Voronoi_diagram). Points are generated randomly. By clicking on any point, it is highlighted and you can move it around the screen. You can also add new points with the left mouse button while holding down the `Left Alt` key. Note that the maximum number of points is 512, but you can increase this number in the code. The initial number of points is 20.

If you want to use Manhattan distance, in the `voronoiColors.frag` file add the line `#define MANHATTAN` (Manhattan distance is only implemented for the color mode).

To enable color mode, you need to add the line `#define COLORS` in the main file.

![](/images/voronoi-diagram-1.png)
![](/images/voronoi-diagram-2.png)

## Using Manhattan distance
![](/images/voronoi-diagram-manhattan-1.png)
![](/images/voronoi-diagram-manhattan-2.png)

## Without colors (only edges)

![](/images/voronoi-diagram-edges-1.png)
![](/images/voronoi-diagram-edges-2.png)

