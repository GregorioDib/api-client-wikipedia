#pragma once

// App info
#define PROGRAM_NAME    "api-client-wikipedia"
#define PROGRAM_VERSION "0.1.0"

// Wikipedia REST API base endpoint
#define WIKIPEDIA_API_BASE "https://en.wikipedia.org/api/rest_v1/page/summary/"

// Local database filename
#define DATABASE_FILE "assets.db"

// External data provider
#define PROVIDER_NAME "wikipedia"

// HTTP User-Agent header used in requests
#define USER_AGENT PROGRAM_NAME "/" PROGRAM_VERSION
