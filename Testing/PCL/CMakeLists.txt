if( NOT BUILD_USE_PCL )
  return()
endif()

#link_directories(${PCL_LIBRARY_DIRS})
include_directories( ${PCL_INCLUDE_DIRS} )
add_definitions( ${PCL_DEFINITIONS} )

if( MSVC )
  set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /IGNORE:4098" )

  # AUTOLINK is used for CGAL on Windows. Disable it for OpenMesh-only tests
  # in order to prevent false link error.
  add_definitions( -DCGAL_NO_AUTOLINK_CGAL )
endif()

# Refer to DevelopersDesignCMakeFiles.dox entry 001
if( BUILD_USE_CGAL )
  include(${CGAL_USE_FILE})
else()
  include_directories( BEFORE "${PROJECT_SOURCE_DIR}/External/CGAL-5.2.1" )
endif()

# Refer to Doc/Devel/CMakeFiles.md entry 002
add_definitions( -DCGAL_NDEBUG )

#####
add_executable( pcd_write_test pcd_write.cpp )
target_link_libraries( pcd_write_test
                       ${PCL_COMMON_LIBRARIES} ${PCL_IO_LIBRARIES}
                       ${Boost_LIBRARIES}
                       )
add_test(pcd_write_test pcd_write_test)

#####
add_executable( test_complying_concepts_pcl
                test_complying_concepts_pcl.cpp )
target_link_libraries( test_complying_concepts_pcl
                       ${PCL_COMMON_LIBRARIES} ${PCL_IO_LIBRARIES}
                       ${Boost_LIBRARIES}
                       )
add_test( Test_complying_concepts_pcl
          test_complying_concepts_pcl )

#####
add_executable( test_property_maps_pcl
                test_property_maps_pcl.cpp )
target_link_libraries( test_property_maps_pcl
                       ${PCL_COMMON_LIBRARIES} ${PCL_IO_LIBRARIES}
                       ${Boost_LIBRARIES}
                       )
add_test( Test_property_maps_pcl
          test_property_maps_pcl )

#####
add_executable( test_reader_writer_pcl
                test_reader_writer_pcl.cpp )
target_link_libraries( test_reader_writer_pcl
                       ${PCL_COMMON_LIBRARIES} ${PCL_IO_LIBRARIES}
                       ${Boost_LIBRARIES}
                       )
add_test( Test_reader_writer_pcl
          test_reader_writer_pcl
          ${TESTING_DATA_DIR}/tetra.with_normal_and_color.ply
          test_reader_writer_pcl.ply
          ${TESTING_REFOUTPUT_DIR}/PCL/test_reader_writer_pcl.ref.ply
          )

#####
add_executable( test_NN_search_concept_pcl
                test_NN_search_concept_pcl.cpp )
target_link_libraries( test_NN_search_concept_pcl
                       ${PCL_LIBRARIES}
                       ${Boost_LIBRARIES}
                       )
add_test( Test_NN_search_concept_pcl
          test_NN_search_concept_pcl )

#####
add_executable( test_point_cloud_curvature_pcl
                test_point_cloud_curvature_pcl.cpp )
target_link_libraries( test_point_cloud_curvature_pcl
                       ${PCL_LIBRARIES}
                       ${Boost_LIBRARIES}
                       )
add_test( Test_point_cloud_curvature_pcl
          test_point_cloud_curvature_pcl
          ${TESTING_DATA_DIR}/casting_with_normals.ply
          test_point_cloud_curvature_pcl_casting.ply
          15
          ${TESTING_REFOUTPUT_DIR}/PCL/test_point_cloud_curvature_pcl_casting.ref.ply
          )

#####
add_executable( test_geometry_concept_pcl
                test_geometry_concept_pcl.cpp )
target_link_libraries( test_geometry_concept_pcl
                       ${PCL_LIBRARIES}
                       ${Boost_LIBRARIES}
                       )
add_test( Test_geometry_concept_pcl
          test_geometry_concept_pcl )

#####
add_executable( test_copy_graph_pcl
                test_copy_graph_pcl.cpp
				        )
target_link_libraries( test_copy_graph_pcl
                       ${PCL_LIBRARIES}
                       ${Boost_LIBRARIES}
                       )
add_test( Test_copy_graph_pcl_monkey_color_normal
          test_copy_graph_pcl
          ${TESTING_DATA_DIR}/monkey_points_with_color_and_normal.ply
          test_copy_graph_pcl_monkey_color_normal.ply
          ${TESTING_REFOUTPUT_DIR}/PCL/test_copy_graph_pcl_monkey_color_normal.ref.ply
          )
