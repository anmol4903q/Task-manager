const list = document.getElementById("list");
const input = document.getElementById("title");

function render(tasks) {
  list.innerHTML = "";
  tasks.forEach(t => {
    const li = document.createElement("li");
    if (t.done) li.className = "done";
    const span = document.createElement("span");
    span.textContent = t.title;           // textContent = safe from HTML injection
    span.onclick = () => call("POST", `/api/tasks/${t.id}/toggle`);
    const del = document.createElement("button");
    del.textContent = "X";
    del.onclick = () => call("DELETE", `/api/tasks/${t.id}`);
    li.append(span, del);
    list.appendChild(li);
  });
  const left = tasks.filter(t => !t.done).length;
  document.getElementById("count").textContent = `${left} task(s) left`;
}

async function call(method, url, body) {
  const res = await fetch(url, { method, body });
  render(await res.json());
}

document.getElementById("add").onclick = () => {
  const title = input.value.trim();
  if (!title) return;
  call("POST", "/api/tasks", new URLSearchParams({ title }));
  input.value = "";
};
input.addEventListener("keydown", e => { if (e.key === "Enter") document.getElementById("add").click(); });

fetch("/api/tasks").then(r => r.json()).then(render);
