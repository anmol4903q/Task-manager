# Deploy to Render

1. Push the contents of this folder to the root of a GitHub repository. Ensure `Dockerfile`, `main.cpp`, `httplib.h`, and `public/` are at the repository root.
2. In Render, choose **New + → Web Service** and connect the GitHub repository.
3. Select **Docker** as the runtime if asked. Keep the Dockerfile path as `./Dockerfile`.
4. Create/deploy the service. The server reads Render's `PORT` environment variable automatically.
5. Open the `onrender.com` URL Render gives you.

Note: `tasks.txt` is local file storage. On Render, local files may be lost when the service restarts or redeploys unless you configure persistent storage. This is suitable for a demo, not durable production data.
