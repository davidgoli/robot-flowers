l = 10;
h = 10;
t = 2;

module grip () {
    difference() {
        square([h, l]);
        translate([t, t, 0]) 
            square([h - t, l - (t * 2)]);
    }
}

module middle (a) {
    x = -(l*cos(a));
    y = (l*sin(a));
    polygon(points = [
        [0,0],
        [0,l],
        [x,y],
    ]);
}

module holder (w, a) {
    linear_extrude(w) {
        grip();
        mirror([1,1,0])
        rotate(a + 90) {
            grip();
        }
        middle(a + 90);
    }
}

holder(w = 50, a = -45);