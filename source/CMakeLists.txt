set(SOURCE_LIST
    "${ASSIGNMENT_SOURCE_ROOT}/main.cpp"
    #"${ASSIGNMENT_SOURCE_ROOT}/sphere.cpp"
    "${ASSIGNMENT_SOURCE_ROOT}/coloursphere.cpp"
    "${ASSIGNMENT_SOURCE_ROOT}/poolscene.cpp"
    "${ASSIGNMENT_SOURCE_ROOT}/staticcollision.cpp"
    "${ASSIGNMENT_SOURCE_ROOT}/movingcollision.cpp"
    "${ASSIGNMENT_SOURCE_ROOT}/quad.cpp"
    "${ASSIGNMENT_SOURCE_ROOT}/colobject.cpp"
    )

set(SHADER_SOURCE "${ASSIGNMENT_SOURCE_ROOT}/AssignmentPaths.cpp")
configure_file("${ASSIGNMENT_SOURCE_ROOT}/AssignmentPaths.cpp.in"
    ${SHADER_SOURCE})

set(ASSIGNMENT_SOURCE_LIST
    ${SOURCE_LIST}
    ${SHADER_SOURCE}
    PARENT_SCOPE)

