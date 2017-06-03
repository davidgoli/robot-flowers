// 22.33 = gear rack holder dist from axle
// 30 = plate dist from axle
// 8 = rack notch depth
// 10 = plate padding before screw holes
// 41 = plate
gear_radius = 16.33;
rack_depth = 6;
plate_dist_from_axle=30;
rack_dist_from_axle = gear_radius + rack_depth;
echo("rack dist from axle:", rack_dist_from_axle);

thickness=2;
length=41; // plate length
width=42; // plate width
slit_width = rack_depth;
slit_length = length;
slit_start_x = 3;
slit_end_x = slit_start_x + slit_width;
slit_start_y = 0;
slit_end_y = slit_start_y + slit_length;
slit_cover_width = slit_width + slit_start_x + slit_width/2;
slit_offset = slit_start_x - slit_width/2;
notch_depth = plate_dist_from_axle-rack_dist_from_axle;

base_points = [[0,0],[width,0],[width,length],[0,length]];
slit_points = [
    [slit_start_x,slit_start_y],
    [slit_end_x,slit_start_y],
    [slit_end_x,slit_end_y],
    [slit_start_x,slit_end_y]
];

hole_padding_x = 5;
hole_padding_y = 5;
hole_width = 6;
hole_length = 3;
hole_x = width - hole_padding_x;
hole1_y = length - hole_padding_y;
hole2_y = hole_padding_y;
hole1 = [
    [hole_x-hole_width,hole1_y-hole_length],
    [hole_x,hole1_y-hole_length],
    [hole_x,hole1_y],
    [hole_x-hole_width,hole1_y]
];

hole2 = [
    [hole_x-hole_width,hole2_y+hole_length],
    [hole_x,hole2_y+hole_length],
    [hole_x,hole2_y],
    [hole_x-hole_width,hole2_y]
];

paths = [[0,1,2,3],[4,5,6,7]];
points = concat(
    base_points,
    slit_points,
    hole1,
    hole2
);

base_paths = concat(paths, [[8,9,10,11],[12,13,14,15]]);

translate([0,plate_dist_from_axle,0]) rotate([-90,0,0]) {
    linear_extrude(thickness) polygon(points=points, paths=base_paths, convexity=10);

    notch_outer_points = [
        [slit_offset,0],
        [slit_cover_width,0],
        [slit_cover_width,length],
        [slit_offset,length]
    ];
    notch_points = concat(
        notch_outer_points,
        slit_points
    );
    translate([0,0,-notch_depth]) {
        linear_extrude(thickness) polygon(notch_outer_points, [[0,1,2,3]]);
        translate([0,0,-thickness]) linear_extrude(notch_depth+thickness) polygon(notch_points, paths);
    }
}