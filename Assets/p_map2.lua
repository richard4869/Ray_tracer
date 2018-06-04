white = gr.material({1.0, 1.0, 1.0}, {0.9, 0.9, 0.9}, 25, 0.0, 0.0, 0.0, 0.0, 0.3,0.5,0,0)
green = gr.material({0.0, 0.9, 0.0}, {0.9, 0.9, 0.0}, 25, 0.0, 0.0, 0.0, 0.0, 0.5,0.8,0,0)
blue = gr.material({0.0, 0.0, 0.9}, {0.0, 0.0, 0.9}, 2, 0.0, 0.0, 0.0, 0.0, 0.3,0.4,0,0)
red = gr.material({1.0, 0.0, 0.0}, {0.0, 0.0, 0.9}, 2, 0.0, 0.0, 0.0, 0.0, 0.2,0.8,0,0)
refl = gr.material({1.0, 1.0, 1.0}, {0.9, 0.9, 0.9}, 25, 1.0, 0.0, 0.0, 0.0, 0.3,0.5,0,0)
rfr = gr.material({1.0, 1.0, 1.0}, {0.9, 0.9, 0.9}, 25, 0.2, 0.8, 1.2, 0.0, 0.3,0.5,0,0)
rof = gr.material({0.8, 0.8, 0.8}, {0.9, 0.9, 0.9}, 25, 0.8, 0.0, 0.0, 0.0, 0.3,0.8,0,1)

scene = gr.node('root')



floor = gr.mesh( 'floor', 'plane.obj' )
scene:add_child(floor)
floor:set_material(white)
floor:scale(61, 60, 60)
floor:translate(0, -30, 0)


c = gr.cylinder('c')
scene:add_child(c)
c:set_material(white)
c:scale(23, 6, 23)
c:translate(0, -20, 0)

c2 = gr.cylinder('c2')
scene:add_child(c2)
c2:set_material(rof)
c2:scale(22.8, 7, 22.8)
c2:translate(0, -20, 0)

l1 = gr.light({0,24,0}, {1.0, 1.0, 1.0}, {1, 1, 0} ,1)

gr.render(scene, 'p_map2.png', 512, 512, 
	  {0, 20, 50,}, {0, -1, -1}, {0, 1, 0}, 80,
	  {0.4, 0.4, 0.4}, {l1, l2})
