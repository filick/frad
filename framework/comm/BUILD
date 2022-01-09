cc_library(
    name = "comm",
    hdrs = ["comm.h"],
    deps = [
        ":channel",
        ":reader",
        ":writer",
        "//common:common"
    ],
    visibility = ["//visibility:public"],
)

cc_test(
    name = "comm_test",
    srcs = ["comm_test.cc"],
    deps = [
        ":comm",
        "@com_google_googletest//:gtest_main"
    ]
)

cc_library(
    name = "writer",
    hdrs = ["writer.h"],
    deps = [
        "//common:common"
    ]
)

cc_library(
    name = "reader",
    hdrs = ["reader.h"],
    deps = [
        "//common:common"
    ]
)

cc_library(
    name = "channel",
    hdrs = ["channel.h"],
    deps = [
        ":reader",
        ":writer",
        "//common:common"
    ]
)

cc_test(
    name = "channel_test",
    srcs = ["channel_test.cc"],
    deps = [
        ":channel",
        "@com_google_googletest//:gtest_main"
    ]
)
