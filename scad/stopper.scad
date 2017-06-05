notch_depth = 5;
length=98;
thickness=2;
base=8;
stopper=20;

rotate([90,0,0]) {
    translate([0,0,-length]) linear_extrude(length) square(notch_depth);
    translate([-1.5,0,0]) {
        linear_extrude(thickness) square(base);
        translate([0,0,-(thickness+length)]) linear_extrude(thickness) square([base,stopper]);
    }
}