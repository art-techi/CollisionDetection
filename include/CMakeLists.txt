set(INCLUDE_LIST
    #"${ASSIGNMENT_INCLUDE_ROOT}/Node.hpp"
    #"${ASSIGNMENT_INCLUDE_ROOT}/sphere.hpp"
    "${ASSIGNMENT_INCLUDE_ROOT}/colobject.hpp"
    "${ASSIGNMENT_INCLUDE_ROOT}/coloursphere.hpp"
    "${ASSIGNMENT_INCLUDE_ROOT}/poolscene.hpp"
    "${ASSIGNMENT_INCLUDE_ROOT}/staticcollision.hpp"
    "${ASSIGNMENT_INCLUDE_ROOT}/movingcollision.hpp"
    "${ASSIGNMENT_INCLUDE_ROOT}/Quad.hpp"

        )

set(SHADER_INCLUDE "${ASSIGNMENT_INCLUDE_ROOT}/AssignmentPaths.hpp")
configure_file("${ASSIGNMENT_INCLUDE_ROOT}/AssignmentPaths.hpp.in"
    ${SHADER_INCLUDE})

set(ASSIGNMENT_INCLUDE_LIST
    ${INCLUDE_LIST}
    ${SHADER_INCLUDE}
    PARENT_SCOPE)
