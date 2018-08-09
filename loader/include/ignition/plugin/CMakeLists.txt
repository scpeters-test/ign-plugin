
install(
  FILES Loader.hh
  DESTINATION ${IGN_INCLUDE_INSTALL_DIR_FULL}/ignition/${IGN_DESIGNATION}
  COMPONENT headers
)

install(
  FILES detail/Loader.hh
  DESTINATION ${IGN_INCLUDE_INSTALL_DIR_FULL}/ignition/${IGN_DESIGNATION}/detail/
  COMPONENT headers
)

