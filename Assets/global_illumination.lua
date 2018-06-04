white = gr.material({1.0, 1.0, 1.0}, {0.8, 0.8, 0.8}, 25, 0.0, 0.0, 0.0, 0.0, 0.3,0.5, 0, 0)
green = gr.material({0.0, 0.9, 0.0}, {0.9, 0.9, 0.0}, 25, 0.0, 0.0, 0.0, 0.0, 0.5,0.8, 0, 0)
blue = gr.material({0.0, 0.0, 0.9}, {0.0, 0.0, 0.9}, 2, 0.0, 0.0, 0.0, 0.0, 0.3,0.4, 0, 0)
red = gr.material({1.0, 0.0, 0.0}, {0.0, 0.0, 0.9}, 2, 0.0, 0.0, 0.0, 0.0, 0.5,0.5, 0, 0)
refl = gr.material({0.1, 0.1, 0.1}, {0.9, 0.9, 0.9}, 25, 0.1, 0.0, 0.0, 0.0, 0.3,0.5, 0, 0)
g_refl = gr.material({0.1, 0.1, 0.1}, {0.9, 0.9, 0.9}, 25, 1.0, 0.0, 0.0, 0.5, 0.3,0.5, 0, 0)
g2_refl = gr.material({0.1, 0.1, 0.1}, {0.9, 0.9, 0.9}, 25, 1.0, 0.0, 0.0, 5.0, 0.3,0.5, 0, 0)
rfr = gr.material({1.0, 1.0, 1.0}, {0.9, 0.9, 0.9}, 25, 0.1, 0.9, 1.52, 0.0, 0.3,0.5, 0, 0)
rof = gr.material({1.0, 1.0, 1.0}, {0.9, 0.9, 0.9}, 25, 0.0, 0.0, 0.0, 0.0, 0.7,0.7, 0, 0)
smoke = gr.material({1.0, 1.0, 1.0}, {0.9, 0.9, 0.9}, 25, 0.0, 0.0, 0.0, 0.0, 0.0,0.0, 1, 0)
n_white = gr.material({1.0, 1.0, 1.0}, {0.8, 0.8, 0.8}, 25, 0.0, 0.0, 0.0, 0.0, 0.5,0.6, 0, 1)

num5 = gr.texture('Assets/texture/num5.png')
num11 = gr.texture('Assets/texture/num11.png')
ground = gr.texture('Assets/texture/ground.png')
plaster = gr.texture('Assets/texture/plaster.png')

scene = gr.node('root')

b_wall = gr.cube( 'b_wall')
scene:add_child(b_wall)
b_wall:set_material(white)
b_wall:scale(30, 61, 1)
b_wall:translate(-15, -30, 0)

l_wall = gr.cube( 'l_wall')
scene:add_child(l_wall)
l_wall:set_material(green)
l_wall:scale(1, 120, 120)
l_wall:translate(-30, -60, -60)

r_wall = gr.cube('r_wall')
scene:add_child(r_wall)
r_wall:set_material(blue)
r_wall:scale(4, 120, 120)
r_wall:translate(30,-60, -60)

roof = gr.mesh( 'roof', 'plane.obj' )
scene:add_child(roof)
roof:set_material(rof)
roof:rotate('z',180.0)
roof:scale(31, 1, 61)
roof:translate(0, 60, 0)

floor = gr.mesh( 'floor', 'plane.obj' )
scene:add_child(floor)
floor:set_material(red)
floor:scale(120, 1, 61)
floor:translate(0, -60, 0)

s = gr.cube('s')
scene:add_child(s)
s:set_material(n_white)
s:scale(10, 10, 10)
s:translate(26, 15, -5)

smoket = gr.mesh( 'smoket','cube.obj')
scene:add_child(smoket)
smoket:set_material(smoke)
smoket:scale(40, 60, 30)
smoket:translate(-20, -30, -10)

l1 = gr.light({45,26,0}, {1.0, 1.0, 1.0}, {1, 1, 0}, 6.0 )

gr.render(scene, 'golbal.png', 512, 512, 
	  {0, 0, 60,}, {0, 0, -1}, {0, 1, 0}, 80,
	  {0.4, 0.4, 0.4}, {l1, l2})
