use <MCAD/regular_shapes.scad>
use <MCAD/nuts_and_bolts.scad>
use <MCAD/polyholes.scad>

height = 40;
hex_rotation = [0,0,90];
module halving_cube () {
  translate([-150,-150,height/2]) cube([300,300,(height/2)+1]);
}

module ring_base () {
    rotate(hex_rotation) linear_extrude(height) {
        difference() {
            hexagon(50);
            hexagon(30);
        }
    }
}

module fulcrum () {
    hull() {
        triangle(12);
        translate([0,9,0]) circle(3);
    };
}

module ring_lever_sink () {
    translate([-5,0,0]) difference() {
        translate([-47,3,5]) rotate([90,0,0]) linear_extrude(6) square([46,40]);
        scale([2,1,1]) {
            translate([-12,3.5,5]) rotate([90,0,0]) linear_extrude(7) fulcrum();
            translate([-12,3.5,35]) rotate([90,180,0]) linear_extrude(7) fulcrum();
        }
    }
}

module ring_peg () {
  h = 5; d = 10;
  translate([0,38,17.5]) polyhole(h,d);
}

module ring_screw_sink () {
  M5 = 5;
  h = 3; d = 5;
  union() {
    translate([0,38,-1]) {
      scale([1,1,5]) nutHole(M5);
      scale([1,1,10]) polyhole(h,d);
    }
  }
}

module ring () {
    difference() {
        ring_base();
        for(i=[1:6]) rotate([0,0,i * 60]) ring_lever_sink();
        for(i=[1:3]) rotate([0,0,i * 120]) ring_screw_sink();
        halving_cube();
    }
}

module effector_core () {
    rotate(hex_rotation) linear_extrude(height) hexagon(29);
}

module effector_screw_sink () {
  M5 = 5;
  h = 3; d = 5;
  union() {
    translate([0,0,-1]) {
      scale([1,1,5]) nutHole(M5);
      scale([1,1,10]) polyhole(h,d);
    }
  }
}

module effector_peg () {
  h = 5; d = 5;
  translate([0,20,17.5]) polyhole(h,d);
}

module lever_peg () {
  translate([-15,5,20]) rotate([90,0,0]) linear_extrude(10) circle(3);
}

module inner_lever_peg () {
  translate([-15,5,20]) rotate([90,0,0]) linear_extrude(10) circle(2.5);
}

module lever_arm() {
  hull() {
    translate([-15,2.5,20]) rotate([90,0,0]) linear_extrude(5) circle(2.5);
    translate([-55,2.5,20]) rotate([90,0,0]) linear_extrude(5) circle(2.5);
  }
}

module lever () {
    inner_lever_peg();
    lever_arm();
}

lever();

module effector_lever_sink () {
  lever_peg();
  translate([-10,0,5]) scale([1,1,1.5]) {
    translate([-12,3,10]) rotate([90,90,0]) linear_extrude(6) fulcrum();
  }
}

module effector () {
  difference() {
    effector_core();
    for(i=[1:6]) rotate([0,0,i * 60]) effector_lever_sink();
    effector_screw_sink();
    halving_cube();
  }
}

module effector_top () {
  union() {
    effector();
    for(i=[1:3]) rotate([0,0,(i * 120) + 60]) effector_peg();
  }
}

module effector_bottom () {
  difference() {
    effector();
    for(i=[1:3]) rotate([0,0,(i * 120) + 60]) effector_peg();
  }
}

module ring_top () {
  union() {
    ring();
    for(i=[1:3]) rotate([0,0,(i * 120) + 60]) ring_peg();
  }
}

module ring_bottom () {
  difference() {
    ring();
    for(i=[1:3]) rotate([0,0,(i * 120) + 60]) ring_peg();
  }
}

ring_bottom();
effector_bottom();
translate([88,0,0]) {
    ring_top();
    effector_top();
}
