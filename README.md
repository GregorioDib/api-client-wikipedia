# api-client-wikipedia

Simple API client worker for consuming the Wikipedia public API and persisting metadata into an SQL database.

---

# Objective

The goal of this project is to build a worker application capable of:

* consuming an external public API,
* processing JSON responses,
* storing metadata persistently in an SQL database.

The assigned provider for this implementation is Wikipedia.

The project aims to emulate a lightweight "second brain" persistence system capable of aggregating heterogeneous external data sources under a common storage model.

---

# API Contract

## Provider

Wikipedia Public REST API.

## Endpoint

```text
https://en.wikipedia.org/api/rest_v1/page/summary/{title}
```

Where `{title}` represents the requested Wikipedia article.

Example:

```text
https://en.wikipedia.org/api/rest_v1/page/summary/Linux
```

---

# Expected API Response

The API returns JSON metadata describing the requested article.

Example response:

```json
{
  "title": "Linux",
  "description": "Operating system kernel",
  "extract": "Linux is a family of open-source Unix-like operating systems...",
  "content_urls": {
    "desktop": {
      "page": "https://en.wikipedia.org/wiki/Linux"
    }
  }
}
```

---

# Persistence Model

The worker stores data using the following SQL schema:

```sql
CREATE TABLE IF NOT EXISTS assets (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    asset_uri TEXT UNIQUE NOT NULL,
    title TEXT,
    entity TEXT NOT NULL,
    provider TEXT NOT NULL,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    meta_payload TEXT
);

CREATE TABLE IF NOT EXISTS assets_history (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    asset_id INTEGER NOT NULL,
    changed_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    history_payload TEXT NOT NULL,
    FOREIGN KEY (asset_id) REFERENCES assets (id) ON DELETE CASCADE
);
```

---

# Stored Fields

| Field        | Description                          |
| ------------ | ------------------------------------ |
| asset_uri    | Unique URI of the article            |
| title        | Article title                        |
| entity       | Logical entity type                  |
| provider     | Data provider (`wikipedia`)          |
| meta_payload | Raw JSON payload returned by the API |

---

# Historical Snapshots

The project also supports historical persistence of asset metadata.

When an existing asset is updated, the previous state may be stored in the `assets_history` table as a JSON snapshot.

This allows:

* basic change tracking,
* metadata history preservation,
* auditing of API data evolution over time.

The original external source is never modified.

---

# Planned Architecture

The application workflow is expected to be:

1. Receive an article title as input.
2. Perform an HTTP request to the Wikipedia API.
3. Receive and parse the JSON response.
4. Extract relevant metadata.
5. Persist the resource into the SQL database.
6. Store the complete JSON payload for future processing.
7. Optionally preserve previous versions in the history table.

---

# Technologies

* C
* HTTP client library
* JSON parser
* SQLite

---

# Current Status

Current repository status:

* repository initialized,
* API contract defined,
* database schema defined,
* persistence model documented,
* architecture under development.

Implementation is currently in progress.

---

# Notes

This project is intended for educational purposes.

The worker acts only as a consumer of public information and does not modify Wikipedia content.

