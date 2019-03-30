# Convex Polygons

The aim of this project is to create a `polygon calculator` in order to be able to do some stuff with convex polygons.
First of all, to compile this programm it is necessary to do the following steps, depending on the OS you use:
(The following part is mostly inspired by Jordi Petit's github, and it is also explained in catalan to ease the understanding)

### LINUX (FIB)

Aneu en algun directori temporal i baixeu-hi la llibreria pngwriter:

`git clone https://github.com/pngwriter/pngwriter.git`

Compileu la llibreria:
```
# entreu al repositori amb el codi font de la llibreria que heu baixat
cd pngwriter

# prepareu la compilació amb algunes opcions
cmake -DPNGwriter_USE_FREETYPE=OFF -DCMAKE_INSTALL_PREFIX=$HOME/libs .

# compileu la llibreria
make

# instal·leu la llibreria
make install
```
A la instrucció cmake, el flag -DPNGwriter_USE_FREETYPE=OFF inhibeix l'ús de les llibreries FreeType (que potser no teniu instal·lades i no necessitem). El flag -DCMAKE_INSTALL_PREFIX=$HOME/libs indica a on s'instal·larà la llibreria (en aquest cas, al directori libs sota el vostre directori principal, referit per la variable d'entorn $HOME).

Un cop compilada i instal·lada la llibreria, el seu codi ja no us cal (perquè el teniu ja en forma de llibreria a $HOME/libs). Per tant, podeu esborrar-la:
```
cd ..
rm -rf pngwriter
```
Als directoris include i lib dins de $HOME/libs veureu el que ha quedat instal·lat (això no ho hauríeu d'esborrar).

### Mac OS

Sembla que en Mac potser cal que us instal·leu els paquets cmake i libpng. Recomanem que utilitzeu el gestor de paquets brew si no el teniu encara:
```
# instal·la brew
/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"

# instal·la cmake i libpng
brew install cmake libpng
```

### Ubuntu

Sembla que en Ubuntu potser cal que us instal·leu aquests paquets:

`sudo apt install cmake libpng16-tools libpng16-devtoolsp`

## Polygon calculator

Once this is done, the project is ready to compile.

Before executing it, there are some important things needed to be known:

- Lines starting with a hash sign (#) are comments. Their output is just a hash sign.

- All commands include polygon identifiers. These are made by words, such as p, p1, p2, or pol_gr.

- Points in the commands are given by two pairs of real numbers, in standard notation, to denote the X and Y coordinates. For instance, 0 0 or 3.14 -5.5. When printed, all real numbers will be formatted with three digits after the decimal dot.

- Colors in the commands are given by three real numbers in [0,1], in standard notation, to denote the RGB color. For instance, 0 0 0 denotes black, 1 0 0 denotes red, and 1 0.64 0 denotes orange.

- Filenames in the commands are made up of words, such as f, pol.txt or some_file_name.pol.

- Commands which would not have an answer on the screen will write "ok".

- There can be some errors the user can do that will be tried to be printed on the screen.

### The `polygon` command

The polygon command associates an identifier with a convex polygon made by a set of zero or more points, given by the user in any order, but always giving first X and then Y. The programm itself will calculate the convex_hull of the given points. If the polygon identifier is new, it will create it. If it already existed, it will overwrite the previous polygon. New polygons are black.

### The `print` command

The print command prints the name and the vertices of a vertices of a given polygon. The output will only contain the vertices in the convex hull of the polygon, in clockwise order, starting from the vertex will lower X (and the vertex with lower Y in case of ties). They will be printed in a single line, with one space separating each value.
(the user has to give the name of the polygon). Ex: print p1

### The `area` command

The area command prints the area of the given polygon (the user has to give the name of the polygon). Ex: area p1

### The `perimeter` command

The perimeter command prints the perimeter of the given polygon (the user has to give the name of the polygon). 
Ex: perimeter p1

### The `vertices` command

The vertices command prints the number of vertices of the convex hull of the given polygon (the user has to give the name of the polygon). Ex: vertices p1

### The `edges` command

The edges command prints the number of edges of the convex hull of the given polygon. (the user has to give the name of the polygon). Ex: edges p1

### The `centroid` command

The centroid command prints the coordinates (first X, then Y)  of the centroid of the given polygon (the user has to give the name of the polygon). Ex: area p1

### The `list` command

The list command lists all polygon identifiers, lexycographically sorted.

### The `save` command

The save command saves the given polygons in a file, overwriting it if it already existed. The contents of the file will be the same as in the print command, with a polygon per line. (the user has to give the name of the file and the polygon identifiers). Ex: save prova.txt p1 p2 p3

### The `load` command

The load command loads the polygons stored in a file, in the same way as polygon, but retrieving the vertices and identifiers from the file. (the user has to give the name of the file). Ex: load file1.txt

### The `setcol` command

The setcol command associates a color to the given polygon. (the user has to give the name of the polygon and the three numbers (see the information about colors at the beggining of the Polygon calculator)). Ex: setcol p1 1 0 0

### The `draw` command

The draw command draws a list of polygons in a PNG file, each one with its associated color. The input should be "draw (file.png) (list_of polygons)". The image should be of 500x500 pixels, with white background and the coordinates of the vertices should be scaled to fit in the 498x498 central part of the image, while preserving the original aspect ratio.

### The `intersection` command

This command may receive two or three parameters:

- When receiving two parameters p1 and p2, p1 will be updated to the intersection of the original p1 and p2.

- When receiving three parameters p1, p2 and p3, p1 will  be updated to the intersection of p2 and p3.

For example: intersection pol p p1.

### The `union` command

Just as the intersection command, but with the convex union of polygons.

### The `inside` command

Given two polygons, the inside command prints `yes` or `not` to tell whether the first is inside the second or not.
For example: inside p1 p2

### The `bbox` command

The bbox command creates a new polygon (whose identifier will be the first parameter read) with the four vertices corresponding to the bounding box of the given polygons.
For example: bbox p4 p1 p2 p3

## Sample of polygon calculator (1)

If the file prova.txt is:
```
p2 1.5 1.5 0.5 0.5 1.5 0.5 0.5 1.5
p3 1 0 0 -1 1 -1 0 0
```

<table>
<tr>
<td>

```
# sample script for the polygon calculator
polygon p1 0 0 1 0 1 1 0 1
print p1
area p1
perimeter p1
vertices p1
centroid p1
save file1.txt p1
load prova.txt
list
print p1
print p2
print p3
union p4 p1 p2
print p4
intersection p5 p1 p2
inside p5 p4
setcol p1 1 0 0
setcol p2 0 1 0
setcol p3 0 0 1
draw image.png p1 p2 p3
bbox p6 p1 p2 p3 p4 p5
print p6
intersection p7 p1 p3
print p7
# some errors
foobar
print p
```

</td>
<td>

```
#
ok
p1 0.000 0.000 0.000 1.000 1.000 1.000 1.000 0.000
1.000
4.000
4
0.500 0.500
ok
ok
p1 p2 p3
p1 0.000 0.000 0.000 1.000 1.000 1.000 1.000 0.000
p2 0.500 0.500 0.500 1.500 1.500 1.500 1.500 0.500
p3 0.000 -1.000 0.000 0.000 1.000 0.000 1.000 -1.000
ok
p4 0.000 0.000 0.000 1.000 0.500 1.500 1.500 1.500 1.500 0.500 1.000 0.000
ok
yes
ok
ok
ok
ok
ok
p6 0.000 -1.000 0.000 1.500 1.500 1.500 1.500 -1.000
ok
p7 0.000 0.000 1.000 0.000
#
error: unrecognized command
error: undefined identifier
```

</td>
</tr>
</table>

And the content of the file1.txt should be:
`p1 0 0 0 1 1 1 1 0`

## Sample of polygon calculator (2)

If the file prova2.txt is:
```
p2 0 0 1 1 1 0
p3 0.5 0 0.5 1 1.5 0 1.5 1
```

<table>
<tr>
<td>

```
# sample script for the polygon calculator
polygon p1 0 0 0 1 1 0
print p1
area p1
perimeter p1
vertices p1
centroid p1
save file2.txt p1
load prova2.txt
list
print p1
print p2
print p3
union p4 p1 p2
print p4
intersection p5 p4 p3
print p5
inside p4 p5
setcol p1 1 0 0
setcol p2 0 1 0
setcol p3 0 0 1
draw image.png p1 p2 p3
bbox p6 p1 p2 p3 p4 p5
print p6
intersection p7 p1 p2
print p7
# some errors
foobar
print p
```

</td>
<td>

```
#
ok
p1 0.000 0.000 0.000 1.000 1.000 0.000
0.500
3.414
3
0.333 0.333
ok
ok
p1 p2 p3
p1 0.000 0.000 0.000 1.000 1.000 0.000
p2 0.000 0.000 1.000 1.000 1.000 0.000
p3 0.500 0.000 0.500 1.000 1.500 1.000 1.500 0.000
ok
p4 0.000 0.000 0.000 1.000 1.000 1.000 1.000 0.000
ok
p5 0.500 0.000 0.500 1.000 1.000 1.000 1.000 0.000
not
ok
ok
ok
ok
ok
p60.000 0.000 0.000 1.000 1.500 1.000 1.500 0.000
ok
p7 0.000 0.000 0.500 0.500 1.000 0.000
#
error: unrecognized command
error: undefined identifier
```

</td>
</tr>
</table>


And the content of file `file2.txt` should be: `p1 0 0 0 1 1 0`
