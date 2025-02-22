
######
add_executable( helloworld_filter_cgal_surface_mesh
                helloworld_filter_cgal_surface_mesh.cpp )
target_link_libraries( helloworld_filter_cgal_surface_mesh
                       ${CGAL_LIBRARY} ${CGAL_3RD_PARTY_LIBRARIES}
                       ${Boost_LIBRARIES} ${FBX_LIBRARY} ${VTK_LIBRARIES}
                       ${IMG-3RDPARTY_LIBRARIES}
                       )

######

add_executable( example_msdm2
                "${PROJECT_SOURCE_DIR}/FEVV/Filters/CGAL/Surface_mesh/msdm2.h"
                example_msdm2.cpp
                )
target_link_libraries( example_msdm2
                       ${CGAL_LIBRARY} ${CGAL_3RD_PARTY_LIBRARIES}
                       ${Boost_LIBRARIES} ${FBX_LIBRARY} ${VTK_LIBRARIES}
                       ${IMG-3RDPARTY_LIBRARIES}
                       )
if( BUILD_USE_VTK )
  target_link_libraries( example_msdm2
						 ${VTK_LIBRARIES} )
endif()

######
add_executable( example_cmdm
                example_CMDM.cpp )
target_link_libraries( example_cmdm
                       ${CGAL_LIBRARY} ${CGAL_3RD_PARTY_LIBRARIES}
                       ${Boost_LIBRARIES} ${FBX_LIBRARY} ${VTK_LIBRARIES}
                       ${IMG-3RDPARTY_LIBRARIES}
                      )

######
add_executable( texture_image_demo_filter_cgal_surface_mesh
                texture_image_demo_filter_cgal_surface_mesh.cpp )
target_link_libraries( texture_image_demo_filter_cgal_surface_mesh
                       ${CGAL_LIBRARY} ${CGAL_3RD_PARTY_LIBRARIES}
                       ${Boost_LIBRARIES} ${FBX_LIBRARY} ${VTK_LIBRARIES}
                       ${IMG-3RDPARTY_LIBRARIES}
                      )

######
add_subdirectory( JND )
