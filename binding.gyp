{
  "targets": [
    {
      "target_name": "binding",
      "sources": [ "src/ifaddrs.cc" ],
      "include_dirs": [ "<!(node -e \"require('nan')\")" ]
    }
  ]
}
