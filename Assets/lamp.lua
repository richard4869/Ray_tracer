white = gr.material({1.0, 1.0, 1.0}, {0.9, 0.9, 0.9}, 25, 0.0, 0.0, 0.0, 0.0, 0.3,0.5, 0, 0)
green = gr.material({0.0, 0.9, 0.0}, {0.9, 0.9, 0.0}, 25, 0.0, 0.0, 0.0, 0.0, 0.5,0.8, 0, 0)
blue = gr.material({0.0, 0.0, 0.9}, {0.0, 0.0, 0.9}, 2, 0.0, 0.0, 0.0, 0.0, 0.3,0.4, 0, 0)
red = gr.material({1.0, 0.0, 0.0}, {0.0, 0.0, 0.9}, 2, 0.0, 0.0, 0.0, 0.0, 0.5,0.5, 0, 0)
refl = gr.material({1.0, 1.0, 1.0}, {0.9, 0.9, 0.9}, 25, 1.0, 0.0, 0.0, 0.0, 0.3,0.5, 0, 0)
rfr = gr.material({1.0, 1.0, 1.0}, {0.9, 0.9, 0.9}, 25, 0.1, 0.9, 1.52, 0.0, 0.3,0.5, 0, 0)
rof = gr.material({1.0, 1.0, 1.0}, {0.9, 0.9, 0.9}, 25, 0.0, 0.0, 0.0, 0.0, 0.7,0.7, 0, 0)
n_white = gr.material({1.0, 1.0, 1.0}, {0.9, 0.9, 0.9}, 25, 0.0, 0.0, 0.0, 0.0, 0.4,0.4, 0, 1)
n_yellow = gr.material({1.0, 1.0, 0.1}, {0.9, 0.9, 0.9}, 25, 0.0, 0.0, 0.0, 0.0, 0.4,0.4, 0, 1)
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

s = gr.sphere('s')
scene:add_child(s)
s:set_material(white)
s:scale(8, 8, 8)
s:translate(14, -22, 0)

s = gr.sphere('s')
scene:add_child(s)
s:set_material(n_yellow)
s:scale(8, 8, 8)
s:translate(14, -18, 0)

s1 = gr.cone('s1')
scene:add_child(s1)
s1:set_material(rfr)
s1:scale(12, 22, 12)
s1:translate(-14, 0, 0)
s1:rotate('x',180.0)

s2 = gr.cone('s2')
scene:add_child(s2)
s2:set_material(white)
s2:rotate('x',180.0)
s2:scale(12, 12, 12)
s2:translate(0, 28, 0)


s2 = gr.cone('s2')
scene:add_child(s2)
s2:set_material(n_white)
s2:rotate('x',180.0)
s2:scale(12, 12, 12)
s2:translate(0, 27, 0)


l1 = gr.light({0,24,0}, {1.0, 1.0, 1.0}, {1, 1, 0} ,2)

gr.render(scene, 'p_map.png', 512, 512, 
	  {0, 0, 60,}, {0, 0, -1}, {0, 1, 0}, 80,
	  {0.4, 0.4, 0.4}, {l1, l2})
