white = gr.material({1.0, 1.0, 1.0}, {0.9, 0.9, 0.9}, 25, 0.0, 0.0, 0.0, 0.0, 0.3,0.5, 0, 0)
green = gr.material({0.0, 0.9, 0.0}, {0.9, 0.9, 0.0}, 25, 0.0, 0.0, 0.0, 0.0, 0.5,0.8, 0, 0)
blue = gr.material({0.0, 0.0, 0.9}, {0.0, 0.0, 0.9}, 2, 0.0, 0.0, 0.0, 0.0, 0.3,0.4, 0, 0)
red = gr.material({1.0, 0.0, 0.0}, {0.0, 0.0, 0.9}, 2, 0.0, 0.0, 0.0, 0.0, 0.5,0.5, 0, 0)
refl = gr.material({1.0, 1.0, 1.0}, {0.9, 0.9, 0.9}, 25, 1.0, 0.0, 0.0, 0.0, 0.3,0.5, 0, 0)
rfr = gr.material({1.0, 1.0, 1.0}, {0.9, 0.9, 0.9}, 25, 0.1, 0.9, 1.52, 0.0, 0.3,0.5, 0, 0)
rof = gr.material({1.0, 1.0, 1.0}, {0.9, 0.9, 0.9}, 25, 0.0, 0.0, 0.0, 0.0, 0.7,0.7, 0, 0)
smoke = gr.material({1.0, 1.0, 1.0}, {0.9, 0.9, 0.9}, 25, 0.0, 0.0, 0.0, 0.0, 0.0,0.0, 1, 0)

scene = gr.node('root')

b_wall = gr.mesh( 'b_wall', 'plane.obj' )
scene:add_child(b_wall)
b_wall:set_material(white)
b_wall:scale(30, 31, 30)
b_wall:rotate('x',90.0)
b_wall:translate(0, 0, -30)

l_wall = gr.mesh( 'l_wall', 'plane.obj' )
scene:add_child(l_wall)
l_wall:set_material(green)
l_wall:scale(30, 31, 30)
l_wall:rotate('z',-90.0)
l_wall:translate(-30, 0, 0)

r_wall = gr.mesh( 'r_wall', 'plane.obj' )
scene:add_child(r_wall)
r_wall:set_material(blue)
r_wall:scale(30, 31, 30)
r_wall:rotate('z',90.0)
r_wall:translate(30, 0, 0)

roof = gr.mesh( 'roof', 'plane.obj' )
scene:add_child(roof)
roof:set_material(rof)
roof:scale(31, 30, 30)
roof:rotate('z',180.0)
roof:translate(0, 30, 0)

floor = gr.mesh( 'floor', 'plane.obj' )
scene:add_child(floor)
floor:set_material(red)
floor:scale(31, 30, 30)
floor:translate(0, -30, 0)

s1 = gr.cube('s1')
scene:add_child(s1)
s1:set_material(white)
s1:scale(12, 12, 12)
s1:translate(-14, -20, 20)

s2 = gr.cube('s2')
scene:add_child(s2)
s2:set_material(white)
s2:scale(8, 8, 8)
s2:translate(-14, 20, 0)

s3 = gr.cube('s3')
scene:add_child(s3)
s3:set_material(white)
s3:scale(22, 22, 22)
s3:translate(8, 0, 20)

l1 = gr.light({0,24,0}, {1.0, 1.0, 1.0}, {1, 1, 0},1.0 )
l2 = gr.light({0,0,60}, {1.0, 1.0, 1.0}, {1, 1, 0},1.0 )

gr.render(scene, 'p_map3.png', 512, 512, 
	  {0, 0, 60,}, {0, 0, -1}, {0, 1, 0}, 80,
	  {0.4, 0.4, 0.4}, {l1, l2})
