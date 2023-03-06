PROGRAM AREA;
{ TIPS program for computing some areas. }
VAR
    HEIGHT: REAL;
    WIDTH: REAL;
    PI: REAL;
    RAREA: REAL;
    SAREA: REAL;
    AREA: REAL;
BEGIN
    PI := 3.14159;
    WRITE('Input height: ');
    READ (HEIGHT);
    WRITE('Input width: ');
    READ (WIDTH);
    { area of rectangle }
    RAREA := HEIGHT * WIDTH;
    { area of ellipse }
    EAREA := PI * WIDTH * WIDTH;
    WRITE('RAREA = ');
    WRITE(RREA);
    WRITE('EAREA = ');
    WRITE(EAREA)
END