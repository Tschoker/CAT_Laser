module rounded_cube(length, width, height, radius){
    hull(){
        for (x = [-(length/2)+radius, length/2-radius]){
            for (y = [-width/2+radius, width/2-radius]){
                for (z = [-height/2+radius, height/2-radius]){
                    $fn=20;
                    translate([x,y,z]) sphere(r=radius);
                }
            }
        }
    }
}

module top_rounded_box(length, width, height, radius, thickness){
    difference() {
        rounded_cube(length, width, height*2, radius);
        translate([0,0,-height/2]) cube([length+10,width+10,height], center=true);
        translate([0,0,(height-thickness)/2]) cube([length-2*thickness, width-2*thickness, height-thickness], center=true);
    }
}



module pcb_clamp(length, width, thickness, overhang, distance, corner, pcb_thickness){
    //bottom
    translate([0,0,-thickness/2]) cube([length+thickness*2,width+thickness*2,thickness], center=true);    
    //clamps
    translate([length/4,-width/2-thickness,0]) rotate(90,[0,0,1]) clamp(length/2, distance+pcb_thickness+overhang+1,thickness, overhang);
    translate([-length/4,width/2+thickness,0]) rotate(-90,[0,0,1]) clamp(length/2, distance+pcb_thickness+overhang+1,thickness, overhang);
    translate([-length/2-thickness,-width/4,0]) clamp(width/2, distance+pcb_thickness+overhang+1,thickness, overhang);
    translate([length/2+thickness,width/4,0]) rotate(180,[0,0,1]) clamp(width/2, distance+pcb_thickness+overhang+1,thickness, overhang);
    //distance holders
    translate([length/2-corner,-width/2,0]) cube([corner,corner,distance]);
    translate([-length/2,-width/2,0]) cube([corner,corner,distance]);
    translate([length/2-corner,width/2-corner,0]) cube([corner,corner,distance]);
    translate([-length/2,width/2-corner,0]) cube([corner,corner,distance]);
}

module clamp(length, height, thickness, overhang){
    rotate(-90,[0,0,1]) rotate(-90,[0,1,0]) linear_extrude(length) polygon([[0,0],[0,thickness],[height-overhang,thickness],[height-overhang,thickness+overhang],[height,thickness],[height,0]]);
}

module box_bottom(length, width, height, radius, thickness){
    rotate(180,[1,0,0]) top_rounded_box(length, width, height, radius, thickness);
    //clamps
    translate([-5,-width/2+thickness+1,-1]) rotate(-90,[0,0,1]) clamp(10,10,1,thickness);
    translate([5,width/2-thickness-1,-1]) rotate(90,[0,0,1]) clamp(10,10,1,thickness);
    //lips
    translate([(length-2*thickness)/2*0.7,-(width/2-thickness),-5]) rotate(-90,[0,1,0]) linear_extrude((length-2*thickness)*0.7) polygon([[0,0],[2,1],[10,1],[10,0]]);
    translate([-(length-2*thickness)/2*0.7,(width/2-thickness),-5]) rotate(180,[0,0,1]) rotate(-90,[0,1,0]) linear_extrude((length-2*thickness)*0.7) polygon([[0,0],[2,1],[10,1],[10,0]]);
    translate([-(length-2*thickness)/2,-(width/2-thickness)*0.7,-5]) rotate(270,[0,0,1]) rotate(-90,[0,1,0]) linear_extrude((width-2*thickness)*0.7) polygon([[0,0],[2,1],[10,1],[10,0]]);
    translate([(length-2*thickness)/2,(width/2-thickness)*0.7,-5]) rotate(-270,[0,0,1]) rotate(-90,[0,1,0]) linear_extrude((width-2*thickness)*0.7) polygon([[0,0],[2,1],[10,1],[10,0]]);
}
module box_top(length, width, height, radius, thickness){
    difference(){
        rotate(180,[1,0,0]) top_rounded_box(length, width, height, radius, thickness);
        //cut out for catching the bottom clamps
        translate([0,0,thickness/2-9]) cube([11,width+10,thickness+1], center=true);
    }
}

module box(length, width, height, radius, thickness){
    box_bottom(length, width, height/2, radius, thickness);
    translate([0,-width-10,0]) box_top(length, width, height/2, radius, thickness);
}




difference(){
    union(){
        box(65,50,40,2,2);
        difference(){
            //clamp
            translate([4.5,0,-18]) pcb_clamp(52,36.5,2,0,4,5,3);
            //remove front part of clamp
            translate([31.5,0,-20]) cube([2,44,10], center=true);
            //Löcher für PCB Halteschrauben
            $fn=20;
            translate([-19,15.75,-23]) cylinder(r=1.5, h=30);
            translate([-19,-15.75,-23]) cylinder(r=1.5, h=30);
        }
        translate([29,20.25,-15]) cube([5,4,7], center=true);
        translate([29,-20.25,-15]) cube([5,4,7], center=true);
        translate([26,-23,-11.5]) linear_extrude(2) polygon([[0,0],[5,0],[5,15]]);
        translate([26,23,-9.5]) rotate(180,[1,0,0]) linear_extrude(2) polygon([[0,0],[5,0],[5,15]]);
    }
    //USB port
    translate([30,0,-2.5]) cube([20,13,8], center=true);
    translate([25,-60,2.5]) cube([20,13,8], center=true);
    //cable access top
    translate([24,-60,-15.5]) cube([8,8,20], center=true);
    //button
    $fn=20;
    translate([24,-45,-23]) cylinder(r=4.5, h=30);
    //center hole top
    translate([-8,-60,-23]) cylinder(r=5, h=30);
    //mounting hole top
    translate([10.5,-78.5,-23]) cylinder(r=2, h=30);
    translate([10.5,-41.5,-23]) cylinder(r=2, h=30);
    translate([-26.5,-78.5,-23]) cylinder(r=2, h=30);
    translate([-26.5,-41.5,-23]) cylinder(r=2, h=30);
}





