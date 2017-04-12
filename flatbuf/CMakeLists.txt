set(FLATBUF_SCHEMA_DIR "${PROJECT_SOURCE_DIR}/flatbuf/schema")
set(FLATBUF_GENERATED_DIR "${PROJECT_SOURCE_DIR}/flatbuf/flatbuf")

file(MAKE_DIRECTORY ${FLATBUF_GENERATED_DIR})
execute_process(COMMAND bash "-c" "cd ${FLATBUF_SCHEMA_DIR}; flatc --cpp --grpc -o ${FLATBUF_GENERATED_DIR} ${FLATBUF_SCHEMA_DIR}/*.fbs")

add_library(endpoint_fbs STATIC
    ${FLATBUF_GENERATED_DIR}/endpoint.grpc.fb.cc
)

target_link_libraries(endpoint_fbs
    grpc++
    grpc++_reflection
    grpc
    pthread
    dl
)
